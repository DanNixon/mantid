#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "MantidKernel/V3D.h"
#include "MantidKernel/Quat.h"

/**
  \class  GLViewport
  \brief  class handling OpenGL Viewport
  \author Chapon Laurent & Srikanth Nagella
  \date   August 2008
  \version 1.0

  Base class for viewport with method to resize and get viewport information as well as the projection system it uses and its dimensions. 

*/
class Viewport
{
public:
	enum ProjectionType{ ORTHO, PERSPECTIVE};
	Viewport(int w, int h, ProjectionType type = ORTHO); ///< Constructor with Width (w) and Height(h) as inputs
	/// Called by the display device when viewport is resized
	void resize(int,int);
	/// Get the viewport width and height.
	void getViewport(int& w, int& h) const;
  /// Return the projection type.
	ProjectionType getProjectionType()const;
  /// Set an orthographic projection.
  void setProjection(double,double,double,double,double,double, ProjectionType type = Viewport::ORTHO);
  /// Apply the projection to OpenGL engine
  void applyProjection() const;
  /// Rotate the model
  void applyRotation() const;
  /// Clear all transforamtions (rotation, translation. scaling)
  void reset();

  /* Rotation */

	/// Call to set the View to X+ direction
	void setViewToXPositive();
	/// Call to set the View to Y+ direction
	void setViewToYPositive();
	/// Call to set the View to Z+ direction
	void setViewToZPositive();
	/// Call to set the View to X- direction
	void setViewToXNegative();
	/// Call to set the View to Y- direction
	void setViewToYNegative();
	/// Call to set the View to Z- direction
	void setViewToZNegative();

  /// Init rotation at a point on the screen
  void initRotationFrom(int a,int b);
  /// Generate a new rotation matrix
  void generateRotationTo(int a,int b);

  /* Zooming */

  /// Init zooming with a point on the screen
  void initZoomFrom( int a, int b );
  /// Generate new zooming factor
  void generateZoomTo(int a, int b);
  /// Generate zooming factor using mouse wheel
  void wheelZoom( int a, int b, int d);

  /* Translation */

	/// Call when the mouse button is pressed to translate
	void initTranslateFrom(int,int);
	/// Call when the mouse is moving during a translation
	void generateTranslationTo(int,int);

  // void getProjection(double&,double&,double&,double&,double&,double&);
	void getInstantProjection(double&,double&,double&,double&,double&,double&)const;
	//void setZoomFactor(double);
	//double getZoomFactor();
	//void setTranslation(double,double);
	//void getTranslation(double&,double&);

protected:
  /// Correct for aspect ratio
  void correctForAspectRatio(double& xmin, double& xmax, double& ymin, double& ymax)const;
  /// Project a point onto a sphere centered at rotation point
  void projectOnSphere(int a, int b, Mantid::Kernel::V3D& point) const;
	/// Generate a 3D point coordinates from coordinates on the viewport.
	void generateTranslationPoint(int x,int y, Mantid::Kernel::V3D& p) const;

  /* Projection */

	ProjectionType m_projectionType; ///< Type of display projection
	int m_width;       ///< Width of the viewport in pixels
	int m_height;      ///< Height of the viewport in pixels
	double m_left;     ///< Ortho/Prespective Projection xmin value (Left side of the x axis)
	double m_right;    ///< Ortho/Prespective Projection xmax value (Right side of the x axis)
	double m_bottom;   ///< Ortho/Prespective Projection ymin value (Bottom side of the y axis)
	double m_top;      ///< Ortho/Prespective Projection ymax value (Top side of the y axis)
	double m_near;     ///< Ortho/Prespective Projection zmin value (Near side of the z axis)
	double m_far;      ///< Ortho/Prespective Projection zmax value (Far side of the z axis)

  /* Trackball rotation */

	/// Previous point selected on sphere
	Mantid::Kernel::V3D m_lastpoint;
	/// Rotation matrix stored as a quaternion
	Mantid::Kernel::Quat m_quaternion;
	/// Rotation matrix (4x4 stored as linear array) used in OpenGL
  mutable double m_rotationmatrix[16];
  /// Rotation speed of the trackball
	double m_rotationspeed;

  /* Zooming */

  double m_zoomFactor;

  /* Translation */

  /// Translation in x direction 
  double m_xTrans;
  /// Translation in y direction 
  double m_yTrans;

};

#endif /*VIEWPORT_H_*/
