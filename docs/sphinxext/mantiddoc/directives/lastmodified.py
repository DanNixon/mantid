from __future__ import (absolute_import, division, print_function)

from .base import AlgorithmBaseDirective #pylint: disable=unused-import


class LastModifiedDirective(AlgorithmBaseDirective):
    required_arguments, optional_arguments = 0, 0
    option_spec = {}

    def run(self):
        """
        The main entry point that docutils calls.
        It calls self.execute to do the main work.
        Derived classes should override execute() and insert
        whatever rst they require with self.add_rst()
        """
        nodes = self.execute()
        if self.rst_lines is not None:
            self.commit_rst()
        return nodes

    def execute(self):
        filename = self.doc_source_filename

        last_modified = self.get_file_last_modified(filename)
        gh_url = self.convert_path_to_github_url(filename)

        self.add_rst("This page was last updated: {} (`source <{}>`_)".format(
            last_modified, gh_url))

        return []


def setup(app):
    """
    Setup the directives when the extension is activated

    Args:
      app: The main Sphinx application object
    """
    app.add_directive('lastmodified', LastModifiedDirective)
