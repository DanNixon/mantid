from __future__ import (absolute_import, division, print_function)

import math
import os
import re

from mantid.api import *
from mantid.kernel import *


class SaveGEMMAUDParamFile(PythonAlgorithm):

    PROP_INPUT_WS = "InputWorkspace"
    PROP_TEMPLATE_FILE = "TemplateFilename"
    PROP_GSAS_PARAM_FILE = "GSASParamFile"
    PROP_OUTPUT_FILE = "OutputFilename"

    BANK_GROUPING = ([0] * 7) + ([1] * 8) + ([2] * 20) + ([3] * 42) + ([4] * 54) + ([5] * 35)
    BANK_PARAMS_LINE = "^INS  [0-9]BNKPAR"
    DIFFRACTOMETER_CONSTANTS_LINE = "^INS  [0-9] ICONS"
    PROFILE_COEFFS_LINE_1 = "^INS  [0-9]PRCF 1"
    PROFILE_COEFFS_LINE_2 = "^INS  [0-9]PRCF 2"

    def category(self):
        return "DataHandling\\Text;Diffraction\\DataHandling"

    def name(self):
        return "SaveGEMMAUDParamFile"

    def summary(self):
        return "Read calibration information from focused workspace and GSAS parameter file, and save to " \
               "MAUD-readable calibration format"

    def PyInit(self):
        self.declareProperty(WorkspaceGroupProperty(name=self.PROP_INPUT_WS,
                                                    defaultValue="",
                                                    direction=Direction.Input),
                             doc="WorkspaceGroup of focused banks")

        self.declareProperty(FileProperty(name=self.PROP_GSAS_PARAM_FILE,
                                          action=FileAction.Load,
                                          defaultValue=""),
                             doc="GSAS parameter file to read diffractometer constants and profile coefficients from")

        self.declareProperty(FileProperty(name=self.PROP_TEMPLATE_FILE,
                                          action=FileAction.Load,
                                          defaultValue=self._find_isis_powder_dir()),
                             doc="Template for the .maud file")

        self.declareProperty(FileProperty(name=self.PROP_OUTPUT_FILE,
                                          action=FileAction.Save,
                                          defaultValue=""),
                             doc="Name of the file to save to")

    def PyExec(self):
        input_ws = mtd[self.getPropertyValue(self.PROP_INPUT_WS)]
        spectrum_numbers = [bank.getSpectrum(0).getSpectrumNo() for bank in input_ws]

        gsas_filename = self.getProperty(self.PROP_GSAS_PARAM_FILE).value
        distances, difas, difcs, tzeros, alpha_zeros, alpha_ones, beta_zeros, beta_ones, sigma_zeros, sigma_ones, \
            sigma_twos = map(lambda param: self._expand_to_texture_bank(param, spectrum_numbers),
                             self._parse_gsas_param_file(gsas_filename))

        two_thetas, phis = zip(*[self._get_two_theta_and_phi(bank) for bank in input_ws])

        with open(self.getProperty(self.PROP_TEMPLATE_FILE).value) as template_file:
            template = template_file.read()

        num_banks = input_ws.getNumberOfEntries()
        with open(self.getProperty(self.PROP_OUTPUT_FILE).value, "w") as output_file:
            ###### !!!!!trying with average bank DIFCS
            average_difcs = [678, 664, 746, 872, 888, 761, 1425, 1405, 1391, 1389, 1390, 1396, 1433, 1426, 2411, 2399,
                             2378, 2363, 2425, 2372, 2383, 2426, 2417, 2423, 3141, 3125, 3106, 3093, 3091, 3098, 3114,
                             3160, 3143, 3161, 4219, 4222, 4239, 4211, 4221, 4204, 4206, 4210, 4207, 4242, 4242, 4229,
                             4226, 4223, 4757, 4761, 4782, 4742, 4752, 4736, 4736, 4741, 4739, 4763, 4776, 4765, 4763,
                             4761, 5274, 5279, 5302, 5257, 5267, 5251, 5249, 5255, 5254, 5271, 5289, 5278, 5278, 5277,
                             6176, 6179, 6178, 6179, 6168, 6161, 6159, 6164, 6162, 6163, 6160, 6166, 6171, 6182, 6178,
                             6172, 6178, 6181, 6661, 6663, 6663, 6661, 6654, 6648, 6647, 6651, 6649, 6651, 6647, 6651,
                             6651, 6664, 6663, 6657, 6662, 6661, 7110, 7127, 7111, 7109, 7106, 7100, 7099, 7104, 7101,
                             7104, 7100, 7101, 7101, 7112, 7112, 7107, 7112, 7110, 9001, 9002, 9005, 9031, 9129, 9109,
                             9051, 9007, 9003, 9005, 9001, 9000, 9000, 9000, 9015, 9015, 9013, 9014, 9017, 9018, 9020,
                             9013, 9012, 9013, 9048, 9143, 9228, 9171, 9271, 9053, 9057, 9119, 9045, 9047]

            output_file.write(template.format(gsas_prm_file=gsas_filename,
                                              inst_counter_bank="Bank1",
                                              bank_ids="\n".join("Bank{}".format(i + 1) for i in range(num_banks)),
                                              #difcs=self._format_param_list(difcs),
                                              difas=self._format_param_list(average_difcs),
                                              #difas=self._format_param_list(difas),
                                              difcs=self._create_empty_param_list(num_banks),
                                              #tzeros=self._format_param_list(tzeros),
                                              tzeros=self._create_empty_param_list(num_banks),
                                              thetas=self._format_param_list(two_thetas),
                                              etas=self._format_param_list(phis),
                                              dists=self._format_param_list(distances),
                                              func_1_alpha_zeros=self._format_param_list(alpha_zeros),
                                              func_1_alpha_ones=self._format_param_list(alpha_ones),
                                              func_1_beta_zeros=self._format_param_list(beta_zeros),
                                              func_1_beta_ones=self._format_param_list(beta_ones),
                                              func_1_sigma_zeros=self._format_param_list(sigma_zeros),
                                              func_1_sigma_ones=self._format_param_list(sigma_ones),
                                              func_1_sigma_twos=self._format_param_list(sigma_twos),
                                              func_2_alpha_zeros=self._create_empty_param_list(num_banks),
                                              func_2_alpha_ones=self._create_empty_param_list(num_banks),
                                              func_2_betas=self._create_empty_param_list(num_banks),
                                              func_2_switches=self._create_empty_param_list(num_banks),
                                              func_2_sigma_zeros=self._create_empty_param_list(num_banks),
                                              func_2_sigma_ones=self._create_empty_param_list(num_banks),
                                              func_2_sigma_twos=self._create_empty_param_list(num_banks),
                                              func_2_gamma_zeros=self._create_empty_param_list(num_banks),
                                              func_2_gamma_ones=self._create_empty_param_list(num_banks),
                                              func_2_gamma_twos=self._create_empty_param_list(num_banks),
                                              function_types=self._create_empty_param_list(num_banks, value="1")
                                              ))

    def _create_empty_param_list(self, num_banks, value="0"):
        # Generate a list of zeroes, as several parameters are just zeroed in the .maud file
        return "\n".join(value for _ in range(num_banks))

    def _expand_to_texture_bank(self, bank_param_list, spectrum_numbers):
        return (bank_param_list[self.BANK_GROUPING[spec_num]] for spec_num in spectrum_numbers)

    def _find_isis_powder_dir(self):
        script_dirs = config["pythonscripts.directories"].split(";")
        diffraction_dir = None
        for dir in script_dirs:
            if "Diffraction" in dir:
                diffraction_dir = dir

        if diffraction_dir is None:
            logger.warning("Could not find default diffraction directory for .maud template file: "
                           "you'll have to find it yourself")
            return ""
        else:
            return os.path.join(diffraction_dir, "isis_powder", "gem_routines", "maud_param_template.maud")

    def _format_param_list(self, param_list):
        return "\n".join(str(param) for param in param_list)

    def _get_two_theta_and_phi(self, bank):
        instrument = bank.getInstrument()
        detector = bank.getDetector(0)

        sample_pos = instrument.getSample().getPos()
        source_pos = instrument.getSource().getPos()
        det_pos = detector.getPos()

        beam_dir = sample_pos - source_pos
        detector_dir = det_pos - sample_pos

        return math.degrees(beam_dir.angle(detector_dir)), math.degrees(detector.getPhi())

    def _parse_gsas_param_file(self, gsas_filename):
        with open(gsas_filename) as prm_file:
            gsas_params_lines = prm_file.read().split("\n")

        distances = []
        difcs = []
        difas = []
        tzeros = []
        alpha_zeros = []
        alpha_ones = []
        beta_zeros = []
        beta_ones = []
        sigma_zeros = []
        sigma_ones = []
        sigma_twos = []
        for line in gsas_params_lines:
            line_items = line.split()
            if re.match(self.BANK_PARAMS_LINE, line):
                distances.append(float(line_items[2]))
            elif re.match(self.DIFFRACTOMETER_CONSTANTS_LINE, line):
                difcs.append(float(line_items[3]))
                difas.append(float(line_items[4]))
                tzeros.append(float(line_items[5]))
            elif re.match(self.PROFILE_COEFFS_LINE_1, line):
                alpha_zeros.append(float(line_items[3]))
                alpha_ones.append(float(line_items[4]))
                beta_zeros.append(float(line_items[5]))
                beta_ones.append(float(line_items[6]))
            elif re.match(self.PROFILE_COEFFS_LINE_2, line):
                sigma_zeros.append(float(line_items[3]))
                sigma_ones.append(float(line_items[4]))
                sigma_twos.append(float(line_items[5]))

        return distances, difas, difcs, tzeros, alpha_zeros, alpha_ones, \
            beta_zeros, beta_ones, sigma_zeros, sigma_ones, sigma_twos

AlgorithmFactory.subscribe(SaveGEMMAUDParamFile)
