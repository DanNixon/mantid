#include "MantidQtAPI/MantidColorMap.h"
#include "MantidQtSliceViewer/ColorBarWidget.h"
#include "MantidQtSliceViewer/QScienceSpinBox.h"
#include "qwt_scale_div.h"
#include <iosfwd>
#include <iostream>
#include <qwt_scale_map.h>
#include <qwt_scale_widget.h>
#include <QKeyEvent>
#include <qwt_plot.h>

//-------------------------------------------------------------------------------------------------
/** Constructor */
ColorBarWidget::ColorBarWidget(QWidget *parent)
: QWidget(parent)
{
  ui.setupUi(this);

  // Default values.
  m_min = 0;
  m_max = 1000;
  m_rangeMin = 0;
  m_rangeMax = 1000;
  m_log = false;
  m_colorMap.changeScaleType( GraphOptions::Linear );

  // Create and add the color bar
  m_colorBar = new QwtScaleWidgetExtended();
  m_colorBar->setToolTip("");
  ui.verticalLayout->insertWidget(2,m_colorBar, 1,0 );

//  QwtPlot * plot = new QwtPlot(this);
//  plot->enableAxis( QwtPlot::xBottom, false);
//  plot->enableAxis( QwtPlot::yLeft, false);
//  plot->enableAxis( QwtPlot::yRight, true);
//  plot->axisScaleDraw(QwtPlot::yRight)->
//  plot->setAxisScale(QwtPlot::yRight, 0, 1e12);
//  ui.verticalLayout->insertWidget(2,plot, 1,0 );


  // Hook up signals
  QObject::connect(ui.checkLog, SIGNAL(stateChanged(int)), this, SLOT(changedLogState(int)));
  QObject::connect(ui.valMin, SIGNAL(editingFinished()), this, SLOT(changedMinimum()));
  QObject::connect(ui.valMax, SIGNAL(editingFinished()), this, SLOT(changedMaximum()));
  QObject::connect(ui.valMin, SIGNAL(valueChangedFromArrows()), this, SLOT(changedMinimum()));
  QObject::connect(ui.valMax, SIGNAL(valueChangedFromArrows()), this, SLOT(changedMaximum()));
  QObject::connect(m_colorBar, SIGNAL(mouseMoved(QPoint, double)), this, SLOT(colorBarMouseMoved(QPoint, double)));

  // Initial view
  this->updateColorMap();
}


//-------------------------------------------------------------------------------------------------
/// @return the minimum value of the min of the color scale
double ColorBarWidget::getMinimum() const
{ return m_min; }

/// @return the maximum value of the max of the color scale
double ColorBarWidget::getMaximum() const
{ return m_max; }

/// @return true if the color scale is logarithmic.
bool ColorBarWidget::getLog() const
{ return m_log; }

/// @return then min/max range currently viewed
QwtDoubleInterval ColorBarWidget::getViewRange() const
{ return QwtDoubleInterval(m_min, m_max); }

/// @return the color map in use (ref)
MantidColorMap & ColorBarWidget::getColorMap()
{ return m_colorMap;
}


//-------------------------------------------------------------------------------------------------
/** Send a double-clicked event but only when clicking the color bar */
void ColorBarWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
  if (m_colorBar->rect().contains(event->x(), event->y()))
    emit colorBarDoubleClicked();
}

//-------------------------------------------------------------------------------------------------
/** Adjust the steps of the spin boxes for log/linear mode */
void ColorBarWidget::setSpinBoxesSteps()
{
  double step = 1.1;
  if (m_log)
  {
    // Logarithmic color scale: move by logarithmic steps
    double logRange;
    if (m_rangeMin <= 0)
    {
      // Try to guess at a valid min range if 0 for log scale
      logRange = log10(m_rangeMax);
      if (logRange >= 3) m_rangeMin = 1;
      else if (logRange >= 0) m_rangeMin = 1e-3;
      // Default to 1/10000 of the max
      else m_rangeMin = pow(10., double(int(logRange))-4.);
    }
    logRange = log10(m_rangeMax) - log10(m_rangeMin);
    if (logRange > 6) logRange = 6;
    step = pow(10., logRange/100.);
  }
  else
  {
    // Linear scale
    // Round step that is between 1/100 to 1/1000)
    int exponent = int(log10(m_rangeMax)) - 2;
    step = pow(10., double(exponent));
  }

  ui.valMin->setMinimum( m_rangeMin );
  ui.valMin->setMaximum( m_rangeMax );
  ui.valMax->setMinimum( m_rangeMin );
  ui.valMax->setMaximum( m_rangeMax );

  if (m_min < m_rangeMin) m_min = m_rangeMin;
  if (m_max > m_rangeMax) m_max = m_rangeMax;

  ui.valMin->setSingleStep(step);
  ui.valMax->setSingleStep(step);
  int dec = 2;
  ui.valMin->setDecimals(dec);
  ui.valMax->setDecimals(dec);

  updateMinMaxGUI();
}


//-------------------------------------------------------------------------------------------------
/** Set the range of values in the overall data (limits to selections)
 * Update the display.
 *
 * @param min
 * @param max
 */
void ColorBarWidget::setDataRange(double min, double max)
{
  m_rangeMin = min;
  m_rangeMax = max;
  setSpinBoxesSteps();
}
void ColorBarWidget::setDataRange(QwtDoubleInterval range)
{ this->setDataRange(range.minValue(), range.maxValue()); }

//-------------------------------------------------------------------------------------------------
/** Set the range of values viewed in the color bar
 *
 * @param min :: min value = start of the color map
 * @param max :: max value = end of the color map
 */
void ColorBarWidget::setViewRange(double min, double max)
{
  m_min = min;
  m_max = max;
  updateMinMaxGUI();
}

void ColorBarWidget::setViewRange(QwtDoubleInterval range)
{ this->setViewRange(range.minValue(), range.maxValue()); }

//-------------------------------------------------------------------------------------------------
/** SLOT called when clicking the log button */
void ColorBarWidget::changedLogState(int log)
{
  this->setLog(log);
  emit changedColorRange(m_min,m_max,m_log);
}

//-------------------------------------------------------------------------------------------------
/** Set the color bar to use log scale
 *
 * @param log :: true to use log scale
 */
void ColorBarWidget::setLog(bool log)
{
  m_log = log;
  m_colorMap.changeScaleType( m_log ? GraphOptions::Log10 : GraphOptions::Linear );
  ui.checkLog->setChecked( m_log );
  ui.valMin->setLogSteps( m_log );
  ui.valMax->setLogSteps( m_log );
  setSpinBoxesSteps();
}

//-------------------------------------------------------------------------------------------------
/** SLOT called when minValue changes */
void ColorBarWidget::changedMinimum()
{
  m_min = ui.valMin->value();
  if (m_min > m_max)
  {
    m_max = m_min+0.001;
    ui.valMax->setValue( m_max );
  }
  emit changedColorRange(m_min,m_max,m_log);
}

//-------------------------------------------------------------------------------------------------
/** SLOT called when maxValue changes */
void ColorBarWidget::changedMaximum()
{
  m_max = ui.valMax->value();
  if (m_max < m_min)
  {
    m_min = m_max-0.001;
    ui.valMin->setValue( m_min );
  }
  emit changedColorRange(m_min,m_max,m_log);
}

//-------------------------------------------------------------------------------------------------
/** SLOT called when the mouse moves over the color bar*/
void ColorBarWidget::colorBarMouseMoved(QPoint globalPos, double fraction)
{
  double val = 0;
  if (m_log)
    val = pow(10., fraction * (log10(m_max)-log10(m_min)) + log10(m_min));
  else
    val = fraction * (m_max-m_min) + m_min;
  QString tooltip = QString::number(val,'g', 4);
  QToolTip::showText(globalPos, tooltip, m_colorBar);
}


//-------------------------------------------------------------------------------------------------
/** Update the widget when the color map is changed */
void ColorBarWidget::updateColorMap()
{
  // The color bar alway shows the same range. Doesn't matter since the ticks don't show up
  QwtDoubleInterval range(1.0, 100.0);
  m_colorBar->setColorBarEnabled(true);
  m_colorBar->setColorMap( range, m_colorMap);
  m_colorBar->setColorBarWidth(15);
  m_colorBar->setEnabled(true);

  QwtScaleDiv scaleDiv;
  scaleDiv.setInterval(range);
  if (m_log)
    m_colorBar->setScaleDiv(new QwtScaleTransformation(QwtScaleTransformation::Log10), scaleDiv);
  else
    m_colorBar->setScaleDiv(new QwtScaleTransformation(QwtScaleTransformation::Linear), scaleDiv);

}

//-------------------------------------------------------------------------------------------------
/** Updatet the widget when changing min/max*/
void ColorBarWidget::updateMinMaxGUI()
{
  ui.valMin->setValue( m_min );
  ui.valMax->setValue( m_max );
}



ColorBarWidget::~ColorBarWidget()
{

}
