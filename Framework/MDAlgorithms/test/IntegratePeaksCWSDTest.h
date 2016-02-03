#ifndef MANTID_MDAGORITHMS_INTEGRATEPEAKSCWSDTEST_H_
#define MANTID_MDAGORITHMS_INTEGRATEPEAKSCWSDTEST_H_

#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/IMDEventWorkspace.h"
#include "MantidMDAlgorithms/IntegratePeaksCWSD.h"
#include "MantidTestHelpers/ComponentCreationHelper.h"
#include "MantidKernel/V3D.h"
#include "MantidDataObjects/MDEventFactory.h"
#include "MantidGeometry/MDGeometry/QSample.h"
#include "MantidDataObjects/MDEventInserter.h"


#include <cxxtest/TestSuite.h>

#include <Poco/File.h>

using Mantid::API::AnalysisDataService;
using Mantid::Geometry::MDHistoDimension;
using namespace Mantid::API;
using namespace Mantid::DataObjects;
using namespace Mantid::Geometry;
using namespace Mantid::Kernel;
using namespace Mantid::MDAlgorithms;
using Mantid::Kernel::V3D;

class IntegratePeaksCWSDTest : public CxxTest::TestSuite {
public:

  static IntegratePeaksCWSDTest *createSuite()
  {
    return new IntegratePeaksCWSDTest();
  }

  static void destroySuite(IntegratePeaksCWSDTest *suite)
  {
    delete suite;
  }

  //-------------------------------------------------------------------------------
  /** Test initialize of the algorithm
   */
  void test_Init() {
    IntegratePeaksCWSD alg;
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
  }

  //-------------------------------------------------------------------------------
  /** Test integrate MDEventWorkspace with 1 run
   */
  void test_singleRun()
  {
    // Initialize algorithm and set up
    IntegratePeaksCWSD alg;
    alg.initialize();
    TS_ASSERT(alg.isInitialized())

    // Create workspaces to test
    std::vector<Mantid::Kernel::V3D> vec_qsample;
    std::vector<double> vec_signal;
    std::vector<int> vec_detid;
    std::vector<int> vec_runnumbers;
    createMDEvents1Run(vec_qsample, vec_signal, vec_detid, vec_runnumbers);

    IMDEventWorkspace_sptr inputws = createMDWorkspace(vec_qsample, vec_signal,
                                                       vec_detid, vec_runnumbers);
    AnalysisDataService::Instance().addOrReplace("TestMDWS", inputws);

    std::vector<int> runnumberlist;
    runnumberlist.push_back(vec_runnumbers[0]);
    Mantid::Kernel::V3D peakcenter(3, 3, 3);
    std::vector<Mantid::Kernel::V3D> peakcenterlist;
    peakcenterlist.push_back(peakcenter);
    PeaksWorkspace_sptr peakws = buildPeakWorkspace(runnumberlist, peakcenterlist);
    AnalysisDataService::Instance().addOrReplace("TestPeaksWS", peakws);


    alg.setProperty("InputWorkspace", inputws);
    alg.setProperty("PeaksWorkspace", peakws);
    alg.setProperty("OutputWorkspace", "IntegratedPeakWS");
    alg.setProperty("PeakRadius", 0.3);

    alg.execute();
    TS_ASSERT(alg.isExecuted())

  }

  //-------------------------------------------------------------------------------
  /** Test integrate MDEventWorkspace with 1 run
   */
  void Xtest_multipleRun()
  {
    // Create workspaces to test
    std::vector<Mantid::Kernel::V3D> vec_qsample;
    std::vector<double> vec_signal;
    std::vector<int> vec_detid;
    std::vector<int> vec_runnumbers;
    createMDEvents2Run(vec_qsample, vec_signal, vec_detid, vec_runnumbers);

    IMDEventWorkspace_sptr inputws = createMDWorkspace(vec_qsample, vec_signal,
                                                       vec_detid, vec_runnumbers);
    AnalysisDataService::Instance().addOrReplace("TestMDWS", inputws);

    std::vector<int> runnumberlist;
    runnumberlist.push_back(vec_runnumbers.front());
    runnumberlist.push_back(vec_runnumbers.back());
    Mantid::Kernel::V3D peakcenter(3, 3, 3);
    std::vector<Mantid::Kernel::V3D> peakcenterlist;
    peakcenterlist.push_back(peakcenter);
    peakcenterlist.push_back(peakcenter);
    PeaksWorkspace_sptr peakws = buildPeakWorkspace(vec_runnumbers, peakcenterlist);
    AnalysisDataService::Instance().addOrReplace("TestPeaksWS", peakws);

    // Initialize algorithm and set up
    IntegratePeaksCWSD alg;
    TS_ASSERT(alg.isInitialized())

    alg.setProperty("InputWorkspace", inputws);
    alg.setProperty("PeaksWorkspace", peakws);
    alg.setProperty("OutputWorkspace", "IntegratedPeakWS");
    alg.setProperty("PeakRadius", 0.2);
    alg.setPropertyValue("PeakCentre", "0, 0, 0");
    alg.setProperty("MergePeaks", true);

    alg.execute();
    TS_ASSERT(alg.isExecuted())

  }


  //-------------------------------------------------------------------------------
  /** Add a list of MDEvents around Q = (1, 2, 3)
   * @brief createMDWorkspace
   */
  IMDEventWorkspace_sptr createMDWorkspace(const std::vector<Mantid::Kernel::V3D> &vec_event_qsample,
                                           const std::vector<double> &vec_event_signal,
                                           const std::vector<int> &vec_event_det,
                                           const std::vector<int> &vec_event_run)
  {
    // Check the inputs
    TS_ASSERT_EQUALS(vec_event_qsample.size(), vec_event_signal.size());
    TS_ASSERT_EQUALS(vec_event_qsample.size(), vec_event_det.size());
    TS_ASSERT_EQUALS(vec_event_qsample.size(), vec_event_run.size());

    // Create MDEVENTWorkspace
    // Create workspace in Q_sample with dimenion as 3
    size_t nDimension = 3;
    IMDEventWorkspace_sptr mdws =
        MDEventFactory::CreateMDWorkspace(nDimension, "MDEvent");

    // Extract Dimensions and add to the output workspace.
    std::vector<std::string> vec_ID(3);
    vec_ID[0] = "Q_sample_x";
    vec_ID[1] = "Q_sample_y";
    vec_ID[2] = "Q_sample_z";
    std::vector<std::string> dimensionNames(3);
    dimensionNames[0] = "Q_sample_x";
    dimensionNames[1] = "Q_sample_y";
    dimensionNames[2] = "Q_sample_z";

    Mantid::Kernel::SpecialCoordinateSystem coordinateSystem =
        Mantid::Kernel::QSample;

    // Add dimensions
    std::vector<double> m_extentMins(3);
    std::vector<double> m_extentMaxs(3);
    std::vector<size_t> m_numBins(3, 100);
    for (size_t i = 0; i < 3; ++i)
    {
      m_extentMins[i] = 2;
      m_extentMaxs[i] = 4;
    }

    // Get MDFrame of Qsample
    Mantid::Geometry::QSample frame;

    for (size_t i = 0; i < nDimension; ++i) {
      std::string id = vec_ID[i];
      std::string name = dimensionNames[i];
      mdws->addDimension(
          Mantid::Geometry::MDHistoDimension_sptr(new Mantid::Geometry::MDHistoDimension(
              id, name, frame, static_cast<Mantid::coord_t>(m_extentMins[i]),
              static_cast<Mantid::coord_t>(m_extentMaxs[i]), m_numBins[i])));
    }

    // Set coordinate system
    mdws->setCoordinateSystem(coordinateSystem);

    // Creates a new instance of the MDEventInserter to output workspace
    MDEventWorkspace<MDEvent<3>, 3>::sptr mdws_mdevt_3 =
        boost::dynamic_pointer_cast<MDEventWorkspace<MDEvent<3>, 3>>(mdws);
    MDEventInserter<MDEventWorkspace<MDEvent<3>, 3>::sptr> inserter(mdws_mdevt_3);

    // Go though each spectrum to conver to MDEvent
    for (size_t iq = 0; iq < vec_event_qsample.size(); ++iq) {
      Mantid::Kernel::V3D qsample = vec_event_qsample[iq];
      std::vector<Mantid::coord_t> millerindex(3);
      millerindex[0] = static_cast<Mantid::coord_t>(qsample.X());
      millerindex[1] = static_cast<Mantid::coord_t>(qsample.Y());
      millerindex[2] = static_cast<Mantid::coord_t>(qsample.Z());

      double signal = vec_event_signal[iq];
      double error = std::sqrt(signal);
      uint16_t runnumber = static_cast<uint16_t>(vec_event_run[iq]);
      Mantid::detid_t detid = vec_event_det[iq];

      // Insert
      inserter.insertMDEvent(
          static_cast<float>(signal), static_cast<float>(error * error),
          static_cast<uint16_t>(runnumber), detid, millerindex.data());
    }

    return mdws;
  }

  //-------------------------------------------------------------------------------
  /** Add a peak at Q = (1, 2, 3)
   * @brief buildPW
   * @return
   */
  PeaksWorkspace_sptr buildPeakWorkspace(std::vector<int> vec_run_number,
                                         std::vector<Mantid::Kernel::V3D> vec_q_sample) {
    // create instrument
    Instrument_sptr inst =
        ComponentCreationHelper::createTestInstrumentRectangular2(1, 10);
    inst->setName("SillyInstrument");

    // create PeaksWorkspace with property
    auto pw = PeaksWorkspace_sptr(new PeaksWorkspace);
    pw->setInstrument(inst);
    std::string val = "value";
    pw->mutableRun().addProperty("TestProp", val);
    std::string val_mon = "3012";
    pw->mutableRun().addProperty("monitor", val_mon);

    // add peaks
    size_t num_peaks = vec_run_number.size();
    TS_ASSERT_EQUALS(num_peaks, vec_q_sample.size());

    for (size_t i_peak = 0; i_peak < num_peaks; ++i_peak)
    {
      Peak p(inst, 1, 3.0);
      Mantid::Kernel::V3D qsample = vec_q_sample[i_peak];
      p.setQSampleFrame(qsample);
      p.setRunNumber(vec_run_number[i_peak]);

      pw->addPeak(p);
    }
    return pw;
  }

  //-------------------------------------------------------------------------------
  void createMDEvents1Run(std::vector<Mantid::Kernel::V3D> &vec_qsample,
                          std::vector<double> &vec_signal,
                          std::vector<Mantid::detid_t> &vec_detid,
                          std::vector<int> &vec_runnumber){

    double q_x0 = -0.4;
    double q_y0 = -0.4;
    double q_z0 = -0.4;
    double d_q = 0.1;
    Mantid::Kernel::V3D origin(0, 0, 0);

    Mantid::detid_t detid = 1000;
    for (size_t i = 0; i < 8; ++i)
    {
      double q_x = static_cast<double>(i) * d_q + q_x0;
      for (size_t j = 0; j < 8; ++j)
      {
        double q_y = static_cast<double>(j) * d_q + q_y0;
        for (size_t k = 0; k < 8; ++k)
        {
          double q_z = static_cast<double>(k) * d_q + q_z0;
          Mantid::Kernel::V3D qsample(q_x, q_y, q_z);
          double signal = qsample.distance(origin) * 1000;

          vec_qsample.push_back(qsample);
          vec_signal.push_back(signal);
          vec_detid.push_back(detid);
          vec_runnumber.push_back(121);

          ++ detid;
        }
      }
    }

    return;
  }

  //-------------------------------------------------------------------------------
  void createMDEvents2Run(std::vector<Mantid::Kernel::V3D> &vec_qsample,
                          std::vector<double> &vec_signal,
                          std::vector<Mantid::detid_t> &vec_detid,
                          std::vector<int> &vec_runnumber){

    double q_x0 = -0.4;
    double q_y0 = -0.4;
    double q_z0 = -0.4;
    double d_q = 0.1;
    Mantid::Kernel::V3D origin(0, 0, 0);

    Mantid::detid_t detid = 1000;
    for (size_t i = 0; i < 8; ++i)
    {
      double q_x = static_cast<double>(i) * d_q + q_x0;
      for (size_t j = 0; j < 8; ++j)
      {
        double q_y = static_cast<double>(j) * d_q + q_y0;
        for (size_t k = 0; k < 8; ++k)
        {
          double q_z = static_cast<double>(k) * d_q + q_z0;
          Mantid::Kernel::V3D qsample(q_x, q_y, q_z);
          double signal = qsample.distance(origin) * 1000;

          vec_qsample.push_back(qsample);
          vec_signal.push_back(signal);
          vec_detid.push_back(detid);
          vec_runnumber.push_back(121);

          ++ detid;
        }
      }
    }

    // Add 2nd run
    q_x0 = -0.3;
    q_y0 = -0.3;
    q_z0 = -0.3;
    d_q = 0.1;

    detid = 1000;
    for (size_t i = 0; i < 8; ++i)
    {
      double q_x = static_cast<double>(i) * d_q + q_x0;
      for (size_t j = 0; j < 8; ++j)
      {
        double q_y = static_cast<double>(j) * d_q + q_y0;
        for (size_t k = 0; k < 8; ++k)
        {
          double q_z = static_cast<double>(k) * d_q + q_z0;
          Mantid::Kernel::V3D qsample(q_x, q_y, q_z);
          double signal = qsample.distance(origin) * 100;

          vec_qsample.push_back(qsample);
          vec_signal.push_back(signal);
          vec_detid.push_back(detid);
          vec_runnumber.push_back(144);

          ++ detid;
        }
      }
    }

    return;
  }

};



#endif /* MANTID_MDEVENTS_INTEGRATEPEAKSCWSDTEST_H_ */
