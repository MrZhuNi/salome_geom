
import geom_pluginsmanager

def about(context):
    from PyQt4.QtGui import QMessageBox
    QMessageBox.about(None, "About GEOM pluginmanager", """GEOM plugins manager in GEOM module """)

geom_pluginsmanager.AddFunction('AboutGEOM','About GEOM plugins manager',about)

