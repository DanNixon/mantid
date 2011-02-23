# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui/sns_data_summary.ui'
#
# Created: Wed Feb 23 10:58:01 2011
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(866, 608)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(Frame.sizePolicy().hasHeightForWidth())
        Frame.setSizePolicy(sizePolicy)
        Frame.setFrameShape(QtGui.QFrame.StyledPanel)
        Frame.setFrameShadow(QtGui.QFrame.Raised)
        self.verticalLayout = QtGui.QVBoxLayout(Frame)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setContentsMargins(-1, -1, -1, 20)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.instr_name_label = QtGui.QLabel(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.instr_name_label.sizePolicy().hasHeightForWidth())
        self.instr_name_label.setSizePolicy(sizePolicy)
        self.instr_name_label.setMinimumSize(QtCore.QSize(100, 30))
        self.instr_name_label.setMaximumSize(QtCore.QSize(100, 100))
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.instr_name_label.setFont(font)
        self.instr_name_label.setObjectName("instr_name_label")
        self.gridLayout_2.addWidget(self.instr_name_label, 0, 0, 1, 1)
        self.label_9 = QtGui.QLabel(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_9.sizePolicy().hasHeightForWidth())
        self.label_9.setSizePolicy(sizePolicy)
        self.label_9.setMinimumSize(QtCore.QSize(200, 30))
        self.label_9.setMaximumSize(QtCore.QSize(200, 30))
        self.label_9.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.label_9.setWordWrap(False)
        self.label_9.setIndent(0)
        self.label_9.setObjectName("label_9")
        self.gridLayout_2.addWidget(self.label_9, 1, 0, 1, 1)
        self.data_file_edit = QtGui.QLineEdit(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.data_file_edit.sizePolicy().hasHeightForWidth())
        self.data_file_edit.setSizePolicy(sizePolicy)
        self.data_file_edit.setMinimumSize(QtCore.QSize(300, 30))
        self.data_file_edit.setMaximumSize(QtCore.QSize(16777215, 30))
        self.data_file_edit.setBaseSize(QtCore.QSize(0, 0))
        self.data_file_edit.setObjectName("data_file_edit")
        self.gridLayout_2.addWidget(self.data_file_edit, 1, 1, 1, 1)
        self.data_file_browse_button = QtGui.QPushButton(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.data_file_browse_button.sizePolicy().hasHeightForWidth())
        self.data_file_browse_button.setSizePolicy(sizePolicy)
        self.data_file_browse_button.setMinimumSize(QtCore.QSize(0, 30))
        self.data_file_browse_button.setMaximumSize(QtCore.QSize(16777215, 30))
        self.data_file_browse_button.setObjectName("data_file_browse_button")
        self.gridLayout_2.addWidget(self.data_file_browse_button, 1, 2, 1, 1)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem, 1, 3, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout_2)
        self.reduction_options_groupbox = QtGui.QGroupBox(Frame)
        self.reduction_options_groupbox.setObjectName("reduction_options_groupbox")
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.reduction_options_groupbox)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setSizeConstraint(QtGui.QLayout.SetFixedSize)
        self.gridLayout.setObjectName("gridLayout")
        self.label_4 = QtGui.QLabel(self.reduction_options_groupbox)
        self.label_4.setObjectName("label_4")
        self.gridLayout.addWidget(self.label_4, 0, 0, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.normalization_none_radio = QtGui.QRadioButton(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.normalization_none_radio.sizePolicy().hasHeightForWidth())
        self.normalization_none_radio.setSizePolicy(sizePolicy)
        self.normalization_none_radio.setMinimumSize(QtCore.QSize(100, 0))
        self.normalization_none_radio.setMaximumSize(QtCore.QSize(100, 16777215))
        self.normalization_none_radio.setObjectName("normalization_none_radio")
        self.horizontalLayout.addWidget(self.normalization_none_radio)
        self.normalization_monitor_radio = QtGui.QRadioButton(self.reduction_options_groupbox)
        self.normalization_monitor_radio.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.normalization_monitor_radio.setObjectName("normalization_monitor_radio")
        self.horizontalLayout.addWidget(self.normalization_monitor_radio)
        self.gridLayout.addLayout(self.horizontalLayout, 0, 1, 1, 1)
        self.label_2 = QtGui.QLabel(self.reduction_options_groupbox)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label = QtGui.QLabel(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(0, 0))
        self.label.setMaximumSize(QtCore.QSize(150, 16777215))
        self.label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label.setObjectName("label")
        self.horizontalLayout_2.addWidget(self.label)
        self.n_q_bins_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        self.n_q_bins_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.n_q_bins_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.n_q_bins_edit.setObjectName("n_q_bins_edit")
        self.horizontalLayout_2.addWidget(self.n_q_bins_edit)
        self.label_3 = QtGui.QLabel(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy)
        self.label_3.setMinimumSize(QtCore.QSize(150, 0))
        self.label_3.setMaximumSize(QtCore.QSize(150, 16777215))
        self.label_3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_2.addWidget(self.label_3)
        self.n_sub_pix_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        self.n_sub_pix_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.n_sub_pix_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.n_sub_pix_edit.setObjectName("n_sub_pix_edit")
        self.horizontalLayout_2.addWidget(self.n_sub_pix_edit)
        self.log_binning_radio = QtGui.QCheckBox(self.reduction_options_groupbox)
        self.log_binning_radio.setObjectName("log_binning_radio")
        self.horizontalLayout_2.addWidget(self.log_binning_radio)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.gridLayout.addLayout(self.horizontalLayout_2, 1, 1, 1, 1)
        self.solid_angle_chk = QtGui.QCheckBox(self.reduction_options_groupbox)
        self.solid_angle_chk.setObjectName("solid_angle_chk")
        self.gridLayout.addWidget(self.solid_angle_chk, 2, 0, 1, 1)
        self.sensitivity_chk = QtGui.QCheckBox(self.reduction_options_groupbox)
        self.sensitivity_chk.setObjectName("sensitivity_chk")
        self.gridLayout.addWidget(self.sensitivity_chk, 3, 0, 1, 1)
        self.sensivity_file_label = QtGui.QLabel(self.reduction_options_groupbox)
        self.sensivity_file_label.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.sensivity_file_label.setWordWrap(False)
        self.sensivity_file_label.setIndent(50)
        self.sensivity_file_label.setObjectName("sensivity_file_label")
        self.gridLayout.addWidget(self.sensivity_file_label, 4, 0, 1, 1)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.sensitivity_file_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_file_edit.sizePolicy().hasHeightForWidth())
        self.sensitivity_file_edit.setSizePolicy(sizePolicy)
        self.sensitivity_file_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.sensitivity_file_edit.setBaseSize(QtCore.QSize(0, 0))
        self.sensitivity_file_edit.setObjectName("sensitivity_file_edit")
        self.horizontalLayout_4.addWidget(self.sensitivity_file_edit)
        self.sensitivity_browse_button = QtGui.QPushButton(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_browse_button.sizePolicy().hasHeightForWidth())
        self.sensitivity_browse_button.setSizePolicy(sizePolicy)
        self.sensitivity_browse_button.setObjectName("sensitivity_browse_button")
        self.horizontalLayout_4.addWidget(self.sensitivity_browse_button)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem2)
        self.gridLayout.addLayout(self.horizontalLayout_4, 4, 1, 1, 1)
        self.sensitivity_range_label = QtGui.QLabel(self.reduction_options_groupbox)
        self.sensitivity_range_label.setIndent(50)
        self.sensitivity_range_label.setObjectName("sensitivity_range_label")
        self.gridLayout.addWidget(self.sensitivity_range_label, 5, 0, 1, 1)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.sensitivity_min_label = QtGui.QLabel(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_min_label.sizePolicy().hasHeightForWidth())
        self.sensitivity_min_label.setSizePolicy(sizePolicy)
        self.sensitivity_min_label.setMinimumSize(QtCore.QSize(0, 0))
        self.sensitivity_min_label.setMaximumSize(QtCore.QSize(50, 16777215))
        self.sensitivity_min_label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sensitivity_min_label.setObjectName("sensitivity_min_label")
        self.horizontalLayout_5.addWidget(self.sensitivity_min_label)
        self.min_sensitivity_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.min_sensitivity_edit.sizePolicy().hasHeightForWidth())
        self.min_sensitivity_edit.setSizePolicy(sizePolicy)
        self.min_sensitivity_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.min_sensitivity_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.min_sensitivity_edit.setObjectName("min_sensitivity_edit")
        self.horizontalLayout_5.addWidget(self.min_sensitivity_edit)
        self.sensitivity_max_label = QtGui.QLabel(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sensitivity_max_label.sizePolicy().hasHeightForWidth())
        self.sensitivity_max_label.setSizePolicy(sizePolicy)
        self.sensitivity_max_label.setMinimumSize(QtCore.QSize(100, 0))
        self.sensitivity_max_label.setMaximumSize(QtCore.QSize(100, 16777215))
        self.sensitivity_max_label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sensitivity_max_label.setObjectName("sensitivity_max_label")
        self.horizontalLayout_5.addWidget(self.sensitivity_max_label)
        self.max_sensitivity_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.max_sensitivity_edit.sizePolicy().hasHeightForWidth())
        self.max_sensitivity_edit.setSizePolicy(sizePolicy)
        self.max_sensitivity_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.max_sensitivity_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.max_sensitivity_edit.setObjectName("max_sensitivity_edit")
        self.horizontalLayout_5.addWidget(self.max_sensitivity_edit)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_5.addItem(spacerItem3)
        self.gridLayout.addLayout(self.horizontalLayout_5, 5, 1, 1, 1)
        self.transmission_chk = QtGui.QCheckBox(self.reduction_options_groupbox)
        self.transmission_chk.setChecked(False)
        self.transmission_chk.setObjectName("transmission_chk")
        self.gridLayout.addWidget(self.transmission_chk, 6, 0, 1, 1)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.transmission_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.transmission_edit.sizePolicy().hasHeightForWidth())
        self.transmission_edit.setSizePolicy(sizePolicy)
        self.transmission_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.transmission_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.transmission_edit.setObjectName("transmission_edit")
        self.horizontalLayout_3.addWidget(self.transmission_edit)
        self.plus_minus_label = QtGui.QLabel(self.reduction_options_groupbox)
        self.plus_minus_label.setObjectName("plus_minus_label")
        self.horizontalLayout_3.addWidget(self.plus_minus_label)
        self.transmission_error_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.transmission_error_edit.sizePolicy().hasHeightForWidth())
        self.transmission_error_edit.setSizePolicy(sizePolicy)
        self.transmission_error_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.transmission_error_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.transmission_error_edit.setObjectName("transmission_error_edit")
        self.horizontalLayout_3.addWidget(self.transmission_error_edit)
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem4)
        self.gridLayout.addLayout(self.horizontalLayout_3, 6, 1, 1, 1)
        self.sampleDetectorDistanceMmLabel = QtGui.QLabel(self.reduction_options_groupbox)
        self.sampleDetectorDistanceMmLabel.setObjectName("sampleDetectorDistanceMmLabel")
        self.gridLayout.addWidget(self.sampleDetectorDistanceMmLabel, 7, 0, 1, 1)
        self.sample_dist_edit = QtGui.QLineEdit(self.reduction_options_groupbox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_dist_edit.sizePolicy().hasHeightForWidth())
        self.sample_dist_edit.setSizePolicy(sizePolicy)
        self.sample_dist_edit.setMinimumSize(QtCore.QSize(80, 0))
        self.sample_dist_edit.setMaximumSize(QtCore.QSize(80, 16777215))
        self.sample_dist_edit.setObjectName("sample_dist_edit")
        self.gridLayout.addWidget(self.sample_dist_edit, 7, 1, 1, 1)
        self.verticalLayout_2.addLayout(self.gridLayout)
        self.verticalLayout.addWidget(self.reduction_options_groupbox)
        spacerItem5 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem5)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.instr_name_label.setText(QtGui.QApplication.translate("Frame", "EQSANS", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("Frame", "Scattering data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.data_file_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter a valid file path to the SANS data file you want to reduce.", None, QtGui.QApplication.UnicodeUTF8))
        self.data_file_browse_button.setText(QtGui.QApplication.translate("Frame", "Browse...", None, QtGui.QApplication.UnicodeUTF8))
        self.reduction_options_groupbox.setTitle(QtGui.QApplication.translate("Frame", "Reduction Options", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("Frame", "Normalization", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_none_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to skip data normalization.", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_none_radio.setText(QtGui.QApplication.translate("Frame", "None", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_monitor_radio.setToolTip(QtGui.QApplication.translate("Frame", "Select to normalize the data to the beam monitor count.", None, QtGui.QApplication.UnicodeUTF8))
        self.normalization_monitor_radio.setText(QtGui.QApplication.translate("Frame", "Monitor", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Frame", "Radial averaging", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Frame", "Number of Q bins  ", None, QtGui.QApplication.UnicodeUTF8))
        self.n_q_bins_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the number of Q bins for the output I(Q) distribution.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Frame", "Number of sub-pixels", None, QtGui.QApplication.UnicodeUTF8))
        self.n_sub_pix_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the number of sub-pixels in each direction of a detector pixel to use for the\n"
"radial averaging. For instance, entering 3 will sub-divide each detector pixel by 3\n"
"in each direction and will create 9 sub-pixels.", None, QtGui.QApplication.UnicodeUTF8))
        self.log_binning_radio.setToolTip(QtGui.QApplication.translate("Frame", "Check to get log binning of I(q).", None, QtGui.QApplication.UnicodeUTF8))
        self.log_binning_radio.setText(QtGui.QApplication.translate("Frame", "Log binning", None, QtGui.QApplication.UnicodeUTF8))
        self.solid_angle_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to perform a solid angle correction.", None, QtGui.QApplication.UnicodeUTF8))
        self.solid_angle_chk.setText(QtGui.QApplication.translate("Frame", "Perform solid angle correction", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to perform a detector sensitivity correction.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_chk.setText(QtGui.QApplication.translate("Frame", "Perform sensitivity correction", None, QtGui.QApplication.UnicodeUTF8))
        self.sensivity_file_label.setText(QtGui.QApplication.translate("Frame", "Sensitivity data file", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_file_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter a valid file path to be used for the detector sensitivity data.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_browse_button.setText(QtGui.QApplication.translate("Frame", "Browse...", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_range_label.setText(QtGui.QApplication.translate("Frame", "Allowed sensitivity range", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_min_label.setText(QtGui.QApplication.translate("Frame", "Min", None, QtGui.QApplication.UnicodeUTF8))
        self.min_sensitivity_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the minimum allowed relative sensitivity for any given pixel.", None, QtGui.QApplication.UnicodeUTF8))
        self.sensitivity_max_label.setText(QtGui.QApplication.translate("Frame", "Max", None, QtGui.QApplication.UnicodeUTF8))
        self.max_sensitivity_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter the maximum allowed relative sensitivity for any given pixel.", None, QtGui.QApplication.UnicodeUTF8))
        self.transmission_chk.setToolTip(QtGui.QApplication.translate("Frame", "Select to calculate transmission and apply transmission correction.", None, QtGui.QApplication.UnicodeUTF8))
        self.transmission_chk.setText(QtGui.QApplication.translate("Frame", "Calculate transmission", None, QtGui.QApplication.UnicodeUTF8))
        self.transmission_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter transmission value.", None, QtGui.QApplication.UnicodeUTF8))
        self.plus_minus_label.setText(QtGui.QApplication.translate("Frame", "+/-", None, QtGui.QApplication.UnicodeUTF8))
        self.transmission_error_edit.setToolTip(QtGui.QApplication.translate("Frame", "Enter error on transmission.", None, QtGui.QApplication.UnicodeUTF8))
        self.sampleDetectorDistanceMmLabel.setText(QtGui.QApplication.translate("Frame", "Sample-detector distance [mm]", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_dist_edit.setToolTip(QtGui.QApplication.translate("Frame", "Sample-detector distance [mm].", None, QtGui.QApplication.UnicodeUTF8))

