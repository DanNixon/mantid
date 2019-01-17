# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +
#pylint: disable=no-init
from __future__ import (absolute_import, division, print_function)

from mantid.api import PythonAlgorithm, AlgorithmFactory, ITableWorkspaceProperty, WorkspaceFactory
from mantid.kernel import Direction
from mantid.simpleapi import *
from mantid import mtd

# Create an empty table workspace to be populated by a python script.


class OptimizeCrystalPlacementByRun(PythonAlgorithm):

    def summary(self):
        return "Optimizes the sample position for each run in a peaks workspace."

    def category(self):
        return 'Crystal\\Corrections'

    def seeAlso(self):
        return [ "OptimizeCrystalPlacement" ]

    def PyInit(self):
        # Declare properties
        self.declareProperty(ITableWorkspaceProperty("InputWorkspace", "", Direction.Input),
                             "The name of the peaks workspace that will be optimized.")
        self.declareProperty("Tolerance", 0.15, "Tolerance of indexing of peaks.")
        self.declareProperty(ITableWorkspaceProperty("OutputWorkspace", "", Direction.Output),
                             "The name of the peaks workspace that will be created.")

    def PyExec(self):
        ws = self.getProperty("InputWorkspace").value
        ws_append = self.getProperty("OutputWorkspace").value
        ws_group = 'ws_group'
        tolerance = self.getProperty("Tolerance").value
        if not ws.sample().hasOrientedLattice():
            FindUBUsingIndexedPeaks(PeaksWorkspace=ws,Tolerance=tolerance)
        num,err=IndexPeaks(PeaksWorkspace=ws,Tolerance=tolerance)
        print('Initial Number indexed: %s error: %s\n'%(num, err))
        stats = StatisticsOfTableWorkspace(InputWorkspace=ws)
        stat_col = stats.column('Statistic')
        minR = int(stats.column('RunNumber')[stat_col.index('Minimum')])
        maxR = int(stats.column('RunNumber')[stat_col.index('Maximum')]) + 1
        
        group = []
        for run in range(minR, maxR):
            FilterPeaks(InputWorkspace=ws, OutputWorkspace=str(run), FilterVariable='RunNumber', FilterValue=run, Operator='=')
            run = mtd[str(run)]
            peaks = run.getNumberPeaks()
            if peaks == 0:
                AnalysisDataService.remove( str(run))
            else:
                group.append(str(run))
        GroupWorkspaces(InputWorkspaces=group, OutputWorkspace=ws_group)
        
        OptimizeCrystalPlacement(PeaksWorkspace=ws_group, ModifiedPeaksWorkspace=ws_group, AdjustSampleOffsets=True, MaxSamplePositionChangeMeters=0.005,MaxIndexingError=tolerance)
        
        CloneWorkspace(InputWorkspace=str(minR),OutputWorkspace=ws_append)
        for run in range(minR+1, maxR):
            if AnalysisDataService.doesExist( str(run)):
                CombinePeaksWorkspaces(LHSWorkspace=ws_append, RHSWorkspace=str(run),OutputWorkspace=ws_append)
                print('Optimized %s sample position: %s\n'%(str(run),mtd[str(run)].getPeak(0).getSamplePos()))
                AnalysisDataService.remove( str(run))
        num,err=IndexPeaks(PeaksWorkspace=ws_append,Tolerance=tolerance)
        print('After Optimization Number indexed: %s error: %s\n'%(num, err))
        AnalysisDataService.remove(ws_group)
    
        self.setProperty("OutputWorkspace", ws_append)


# Register algorithm with Mantid
AlgorithmFactory.subscribe(OptimizeCrystalPlacementByRun)
