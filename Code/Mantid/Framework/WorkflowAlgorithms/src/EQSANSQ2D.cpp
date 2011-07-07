//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "MantidWorkflowAlgorithms/EQSANSQ2D.h"
#include "MantidAPI/WorkspaceValidators.h"

namespace Mantid
{
namespace Algorithms
{

// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(EQSANSQ2D)

/// Sets documentation strings for this algorithm
void EQSANSQ2D::initDocs()
{
  this->setWikiSummary("Workflow algorithm to process a reduced EQSANS workspace and produce I(Qx,Qy).");
  this->setOptionalMessage("Workflow algorithm to process a reduced EQSANS workspace and produce I(Qx,Qy).");
}

using namespace Kernel;
using namespace API;
using namespace Geometry;

void EQSANSQ2D::init()
{
  CompositeValidator<> *wsValidator = new CompositeValidator<>;
  wsValidator->add(new WorkspaceUnitValidator<>("Wavelength"));
  declareProperty(new WorkspaceProperty<>("InputWorkspace","",Direction::Input,wsValidator),
      "Workspace to calculate I(qx,Qy) from");
  declareProperty("OutputWorkspace","",Direction::Input);
  declareProperty("NumberOfBins", 100,
      "Number of bins in each dimension of the 2D output", Kernel::Direction::Input);
}

/// Returns the value of a run property from a given workspace
/// @param inputWS :: input workspace
/// @param pname :: name of the property to retrieve
double getRunProperty(MatrixWorkspace_sptr inputWS, const std::string& pname)
{
  Mantid::Kernel::Property* prop = inputWS->run().getProperty(pname);
  Mantid::Kernel::PropertyWithValue<double>* dp = dynamic_cast<Mantid::Kernel::PropertyWithValue<double>* >(prop);
  return *dp;
}

/// Execute algorithm
void EQSANSQ2D::exec()
{
  Progress progress(this,0.0,1.0,3);
  progress.report("Setting up I(qx,Qy) calculation");

  MatrixWorkspace_sptr inputWS = getProperty("InputWorkspace");
  const int nbins = getProperty("NumberOfBins");

  // If the OutputWorkspace property was not given, use the
  // name of the input workspace as the base name for the output
  std::string outputWSName = getPropertyValue("OutputWorkspace");
  if (outputWSName.size()==0)
  {
    outputWSName = inputWS->getName();
  }

  // Determine whether we need frame skipping or not by checking the chopper speed
  bool frame_skipping = false;
  if (inputWS->run().hasProperty("is_frame_skipping"))
  {
    Mantid::Kernel::Property* prop = inputWS->run().getProperty("is_frame_skipping");
    Mantid::Kernel::PropertyWithValue<int>* dp = dynamic_cast<Mantid::Kernel::PropertyWithValue<int>* >(prop);
    frame_skipping = (*dp==1);
  }

  // Get run properties necessary to calculate the input parameters to Qxy
  const double sample_detector_distance = getRunProperty(inputWS, "sample_detector_distance");

  const double nx_pixels = inputWS->getInstrument()->getNumberParameter("number-of-x-pixels")[0];
  const double ny_pixels = inputWS->getInstrument()->getNumberParameter("number-of-y-pixels")[0];
  const double pixel_size_x = inputWS->getInstrument()->getNumberParameter("x-pixel-size")[0];
  const double pixel_size_y = inputWS->getInstrument()->getNumberParameter("y-pixel-size")[0];

  const double beam_ctr_x = getRunProperty(inputWS, "beam_center_x");
  const double beam_ctr_y = getRunProperty(inputWS, "beam_center_y");

  double dxmax = pixel_size_x*std::max(beam_ctr_x,nx_pixels-beam_ctr_x);
  double dymax = pixel_size_y*std::max(beam_ctr_y,ny_pixels-beam_ctr_y);
  double maxdist = std::max(dxmax, dymax);

  // Wavelength bandwidths
  const double wavelength_min = getRunProperty(inputWS, "wavelength_min");
  const double wavelength_max = getRunProperty(inputWS, "wavelength_max");
  double qmax = 4*M_PI/wavelength_min*std::sin(0.5*std::atan(maxdist/sample_detector_distance));

  if (frame_skipping)
  {
    // In frame skipping mode, treat each frame separately
    const double wavelength_min_f2 = getRunProperty(inputWS, "wavelength_min_frame2");
    const double wavelength_max_f2 = getRunProperty(inputWS, "wavelength_max_frame2");

    // Frame 1
    IAlgorithm_sptr cropAlg = createSubAlgorithm("CropWorkspace", .4, .5);
    cropAlg->setProperty<MatrixWorkspace_sptr>("InputWorkspace", inputWS);
    cropAlg->setProperty<double>("XMin", wavelength_min);
    cropAlg->setProperty<double>("XMax", wavelength_max);
    cropAlg->executeAsSubAlg();

    IAlgorithm_sptr qxyAlg = createSubAlgorithm("Qxy", .5, .7);
    qxyAlg->setProperty<MatrixWorkspace_sptr>("InputWorkspace", cropAlg->getProperty("OutputWorkspace"));
    qxyAlg->setProperty<double>("MaxQxy", qmax);
    qxyAlg->setProperty<double>("DeltaQ", qmax/nbins);
    qxyAlg->setProperty<bool>("SolidAngleWeighting", false);
    qxyAlg->executeAsSubAlg();

    std::string outputWSName_frame = outputWSName+"_Iqxy_frame1";
    declareProperty(new WorkspaceProperty<>("OutputWorkspaceFrame1", outputWSName_frame, Direction::Output));
    MatrixWorkspace_sptr result = qxyAlg->getProperty("OutputWorkspace");
    setProperty("OutputWorkspaceFrame1", result);

    // Frame 2
    cropAlg = createSubAlgorithm("CropWorkspace", .7, .8);
    cropAlg->setProperty<MatrixWorkspace_sptr>("InputWorkspace", inputWS);
    cropAlg->setProperty<double>("XMin", wavelength_min_f2);
    cropAlg->setProperty<double>("XMax", wavelength_max_f2);
    cropAlg->executeAsSubAlg();

    qxyAlg = createSubAlgorithm("Qxy", .8, 1.0);
    qxyAlg->setProperty<MatrixWorkspace_sptr>("InputWorkspace", cropAlg->getProperty("OutputWorkspace"));
    qxyAlg->setProperty<double>("MaxQxy", qmax);
    qxyAlg->setProperty<double>("DeltaQ", qmax/nbins);
    qxyAlg->setProperty<bool>("SolidAngleWeighting", false);
    qxyAlg->executeAsSubAlg();

    outputWSName_frame = outputWSName+"_Iqxy_frame2";
    declareProperty(new WorkspaceProperty<>("OutputWorkspaceFrame2", outputWSName_frame, Direction::Output));
    result = qxyAlg->getProperty("OutputWorkspace");
    setProperty("OutputWorkspaceFrame2", result);
  } else {
    // When not in frame skipping mode, simply run Qxy
    IAlgorithm_sptr qxyAlg = createSubAlgorithm("Qxy", .3, 1.0);
    qxyAlg->setProperty<MatrixWorkspace_sptr>("InputWorkspace", inputWS);
    qxyAlg->setProperty<double>("MaxQxy", qmax);
    qxyAlg->setProperty<double>("DeltaQ", qmax/nbins);
    qxyAlg->setProperty<bool>("SolidAngleWeighting", false);
    qxyAlg->executeAsSubAlg();

    outputWSName += "_Iqxy";
    declareProperty(new WorkspaceProperty<>("OutputWorkspaceFrame1", outputWSName, Direction::Output));
    MatrixWorkspace_sptr result = qxyAlg->getProperty("OutputWorkspace");
    setProperty("OutputWorkspaceFrame1", result);
  }
}

} // namespace Algorithms
} // namespace Mantid

