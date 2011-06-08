from PyQt4 import QtGui, uic, QtCore
import reduction_gui.widgets.util as util
import os
from reduction_gui.reduction.sans.eqsans_background_script import Background
from reduction_gui.settings.application_settings import GeneralSettings
from reduction_gui.widgets.base_widget import BaseWidget
from eqsans_sample_data import EQSANSBeamHole
from hfir_sample_data import DirectBeam
import ui.sans.ui_eqsans_background

class BckDirectBeam(DirectBeam):
    
    def __init__(self, parent=None, state=None, settings=None, data_type=None, data_proxy=None):
        super(BckDirectBeam, self).__init__(parent, state, settings, data_type, data_proxy=data_proxy)
        
        if state is None:
            self.set_state(Background.DirectBeam())      
           
    def get_state(self):
        direct_beam = super(BckDirectBeam, self).get_state()
        m = Background.DirectBeam(direct_beam)
        return m
    
    def set_state(self, state):
        super(BckDirectBeam, self).set_state(state)        

class BckBeamHole(BaseWidget):
    
    def __init__(self, parent=None, state=None, settings=None, data_type=None, data_proxy=None):
        super(BckBeamHole, self).__init__(parent, state, settings, data_type, data_proxy=data_proxy)
        
        if state is None:
            self.set_state(Background.BeamHole())      
           
    def get_state(self):
        beam_hole = super(BckBeamHole, self).get_state()
        m = Background.BeamHole(beam_hole)
        return m
    
class BackgroundWidget(BaseWidget):    
    """
        Widget that presents the transmission options to the user
    """
    _method_box = None

    ## Widget name
    name = "Background"      
    
    def __init__(self, parent=None, state=None, settings=None, show_transmission=True, data_type=None, data_proxy=None):
        super(BackgroundWidget, self).__init__(parent, state, settings, data_type, data_proxy=data_proxy) 

        class BckFrame(QtGui.QFrame, ui.sans.ui_eqsans_background.Ui_Frame): 
            def __init__(self, parent=None):
                QtGui.QFrame.__init__(self, parent)
                self.setupUi(self)
                
        self._content = BckFrame(self)
        self._layout.addWidget(self._content)
        
        # Flag to show transmission options or not
        self.show_transmission = show_transmission

        self.initialize_content()
        
        if state is not None:
            self.set_state(state)
        else:
            self.set_state(Background())
            
        self._last_direct_state = None
        self._last_spreader_state = None
    
    def initialize_content(self):
        """
            Declare the validators and event connections for the 
            widgets loaded through the .ui file.
        """
        # Validators
        self._content.transmission_edit.setValidator(QtGui.QDoubleValidator(self._content.transmission_edit))
        self._content.dtransmission_edit.setValidator(QtGui.QDoubleValidator(self._content.dtransmission_edit))
        
        # Connections
        self.connect(self._content.calculate_trans_chk, QtCore.SIGNAL("clicked(bool)"), self._calculate_clicked)
        self.connect(self._content.trans_direct_chk, QtCore.SIGNAL("clicked()"), self._direct_beam)
        self.connect(self._content.beamstop_chk, QtCore.SIGNAL("clicked()"), self._beam_hole)

        self.connect(self._content.background_chk, QtCore.SIGNAL("clicked(bool)"), self._background_clicked)
        self.connect(self._content.background_browse, QtCore.SIGNAL("clicked()"), self._background_browse)
        self.connect(self._content.trans_dark_current_button, QtCore.SIGNAL("clicked()"), self._trans_dark_current_browse)

        self.connect(self._content.background_plot_button, QtCore.SIGNAL("clicked()"), self._background_plot_clicked)
        self.connect(self._content.trans_dark_current_plot_button, QtCore.SIGNAL("clicked()"), self._trans_dark_current_plot_clicked)
        
        # Process transmission option
        if not self.show_transmission:
            self._content.calculate_trans_chk.hide()
            self._content.bck_trans_label.hide()
            self._content.bck_trans_err_label.hide()
            self._content.transmission_edit.hide()
            self._content.dtransmission_edit.hide()
            self._content.calculate_trans_chk.hide()
            self._content.theta_dep_chk.hide()
            self._content.trans_direct_chk.hide()
            self._content.trans_dark_current_label.hide()
            self._content.trans_dark_current_edit.hide()
            self._content.trans_dark_current_button.hide()

        if not self._in_mantidplot:
            self._content.background_plot_button.hide()
            self._content.trans_dark_current_plot_button.hide()

    def _background_plot_clicked(self):
        self.show_instrument(file_name=self._content.background_edit.text)

    def _trans_dark_current_plot_clicked(self):
        self.show_instrument(file_name=self._content.trans_dark_current_edit.text)

    def set_state(self, state):
        """
            Populate the UI elements with the data from the given state.
            @param state: Transmission object
        """
        bck_file = str(self._content.background_edit.text()).strip()
        self._content.background_chk.setChecked(state.background_corr)
        self._content.background_edit.setText(QtCore.QString(state.background_file))
        if state.background_file.strip() != bck_file:
            self.get_data_info()
        self._background_clicked(state.background_corr)

        if self.show_transmission:
            self._content.transmission_edit.setText(QtCore.QString("%6.4f" % state.bck_transmission))
            self._content.dtransmission_edit.setText(QtCore.QString("%6.4f" % state.bck_transmission_spread))
                    
            if isinstance(state.trans_calculation_method, state.DirectBeam):
                self._content.trans_direct_chk.setChecked(True)
                self._direct_beam(state=state.trans_calculation_method)
            else:
                self._content.beamstop_chk.setChecked(True)
                self._beam_hole(state=state.trans_calculation_method)
    
            self._content.calculate_trans_chk.setChecked(state.calculate_transmission)
            self._content.theta_dep_chk.setChecked(state.theta_dependent)
            self._content.trans_dark_current_edit.setText(QtCore.QString(str(state.trans_dark_current)))
            self._calculate_clicked(state.calculate_transmission)
        
        
    def get_state(self):
        """
            Returns an object with the state of the interface
        """
        m = Background()
        
        #m.dark_current_corr = self._content.dark_current_chk.isChecked()
        #m.dark_current_file = unicode(self._content.dark_current_edit.text())
        
        m.background_corr = self._content.background_chk.isChecked()
        m.background_file = str(self._content.background_edit.text())
        
        m.bck_transmission_enabled = self.show_transmission
        if self.show_transmission:
            m.bck_transmission = util._check_and_get_float_line_edit(self._content.transmission_edit)
            m.bck_transmission_spread = util._check_and_get_float_line_edit(self._content.dtransmission_edit)
            m.calculate_transmission = self._content.calculate_trans_chk.isChecked()
            m.theta_dependent = self._content.theta_dep_chk.isChecked()
            m.trans_dark_current = self._content.trans_dark_current_edit.text()
        
            if self._method_box is not None:
                m.trans_calculation_method=self._method_box.get_state()   
        return m

    def _trans_dark_current_browse(self):
        fname = self.data_browse_dialog()
        if fname:
            self._content.trans_dark_current_edit.setText(fname)      
        
    def _direct_beam(self, state=None):
        if state is None:
            state = self._last_direct_state
        if self.show_transmission:
            self._replace_method(BckDirectBeam(self, state=state, settings=self._settings, 
                                               data_type=self._data_type, data_proxy=self._data_proxy))
        
        self._content.trans_dark_current_label.setEnabled(True)
        self._content.trans_dark_current_edit.setEnabled(True)
        self._content.trans_dark_current_button.setEnabled(True)
        
    def _beam_hole(self, state=None):
        if state is None:
            state = self._last_direct_state
        self._replace_method(BckBeamHole(self, state=state, settings=self._settings, 
                                            data_type=self._data_type, data_proxy=self._data_proxy))

        self._content.trans_dark_current_label.setEnabled(False)
        self._content.trans_dark_current_edit.setEnabled(False)
        self._content.trans_dark_current_button.setEnabled(False)


    def _replace_method(self, widget):
        if self._method_box is not None:
            for i in range(0, self._content.widget_placeholder.count()):
                item = self._content.widget_placeholder.itemAt(i)
                self._content.widget_placeholder.removeItem(self._content.widget_placeholder.itemAt(i))
                item.widget().deleteLater()
        self._method_box = widget
        self._content.widget_placeholder.addWidget(self._method_box)
        
    def _background_clicked(self, is_checked):
        self._content.background_edit.setEnabled(is_checked)
        self._content.background_browse.setEnabled(is_checked)
        self._content.background_plot_button.setEnabled(is_checked)
        self._content.calculate_trans_chk.setEnabled(is_checked)
        self._content.theta_dep_chk.setEnabled(is_checked)
        self._content.bck_trans_label.setEnabled(is_checked)
        self._content.bck_trans_err_label.setEnabled(is_checked)
        self._content.transmission_grpbox.setEnabled(is_checked)
        
        self._calculate_clicked(is_checked and self._content.calculate_trans_chk.isChecked())
        
    def _background_browse(self):
        fname = self.data_browse_dialog()
        if fname:
            bck_file = str(self._content.background_edit.text()).strip()
            self._content.background_edit.setText(fname)   
            if str(fname).strip() != bck_file:
                #self.get_data_info()
                pass
               
    def _calculate_clicked(self, is_checked):
        self._content.trans_direct_chk.setEnabled(is_checked)
        self._content.beamstop_chk.setEnabled(is_checked)
        if self._method_box is not None:
            self._method_box.setEnabled(is_checked)
            
        self._content.transmission_edit.setEnabled(not is_checked and self._content.background_chk.isChecked())
        self._content.dtransmission_edit.setEnabled(not is_checked and self._content.background_chk.isChecked())
        
        self._content.trans_dark_current_label.setEnabled(is_checked)
        self._content.trans_dark_current_edit.setEnabled(is_checked)
        self._content.trans_dark_current_button.setEnabled(is_checked)
        self._content.trans_dark_current_plot_button.setEnabled(is_checked)
        
        is_beamstop = self._content.beamstop_chk.isChecked()
        self._content.trans_dark_current_label.setEnabled(is_checked and not is_beamstop)
        self._content.trans_dark_current_edit.setEnabled(is_checked and not is_beamstop)
        self._content.trans_dark_current_button.setEnabled(is_checked and not is_beamstop)
        
    def get_data_info(self):
        """
            Retrieve information from the data file and update the display
        """
        if self._data_proxy is None:
            return
        
        fname = str(self._content.background_edit.text())
        if len(str(fname).strip())>0:
            dataproxy = self._data_proxy(fname, "__background_raw")
            if len(dataproxy.errors)>0:
                #QtGui.QMessageBox.warning(self, "Error", dataproxy.errors[0])
                return
            
            self._settings.last_data_ws = dataproxy.data_ws
            if dataproxy.sample_detector_distance is not None:
                self._content.sample_dist_edit.setText(QtCore.QString(str(dataproxy.sample_detector_distance)))
                util._check_and_get_float_line_edit(self._content.sample_dist_edit, min=0.0)
                 