#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt4 import QtGui, QtCore
from treeModel import TreeModel
import sys, signal, math
from guisubautomata import GuiSubautomata
from gui.runtimeGui import Ui_visualHFSM
from gui.additionalSubautWind import Ui_SubautomataWindow


class AdditionalWindow(QtGui.QWidget, Ui_SubautomataWindow):
	def __init__(self, automatagui, scene, subautomata):
		QtGui.QWidget.__init__(self)
		self.setupUi(self)
		automatagui.nextWindowId += 1
		self.id = automatagui.nextWindowId
		self.scene = scene
		self.subautomata = subautomata
		self.automatagui = automatagui

	def closeEvent(self, event):
		self.automatagui.removeWindow(self)
		self.close()


class AutomataGui(QtGui.QMainWindow, Ui_visualHFSM):

	activeNodeSignal = QtCore.pyqtSignal(str)

	def __init__(self, parent=None):
		QtGui.QMainWindow.__init__(self, parent)
		self.setupUi(self)

		self.subautomataList = []
		self.additionalWindows = []
		self.currentSubautomata = None
		self.selectedNodeId = 0
		self.nextWindowId = 0

		self.schemaScene = QtGui.QGraphicsScene()
		self.schemaView.setScene(self.schemaScene)

		#Tree view
		self.treeModel = TreeModel()
		self.treeView.setModel(self.treeModel)
		self.treeView.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
		self.treeView.customContextMenuRequested.connect(self.createMenu)

		self.lastExpanded = QtCore.QModelIndex()

		self.activeNodeSignal.connect(self.notifySetNodeAsActiveReceived)
		self.autofocus.stateChanged.connect(self.autofocusChanged)

		QtCore.QObject.connect(self.upButton, QtCore.SIGNAL("clicked()"), self.upButtonClicked)
		QtCore.QObject.connect(self.treeView, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.rowClicked)


	def setAutomata(self, subautomataList):
		self.subautomataList = subautomataList


	def isFirstActiveNode(self, node):
		nodeAux = node
		subAux = self.currentSubautomata

		while nodeAux != None:
			if not nodeAux.isInit:
				return False
			idNodeFather = nodeAux.subautomata.idNodeFather
			nodeAux = self.getNode(idNodeFather)

		return True


	def loadAutomata(self):
		for subautomata in self.subautomataList:
			self.currentSubautomata = subautomata

			for node in subautomata.nodeList:
				if node.isInit:
					self.currentSubautomata.setActiveNode(node.name)

				if self.isFirstActiveNode(node):
					color = "green"
					node.setColor(color)
				else:
					color = "white"
				self.createNewState(node, color)

			transList = subautomata.transList
			for trans in transList:
				self.createNewTransition(trans)
			
			if subautomata.idNodeFather != 0:
				subautomata.hide()

		if self.currentSubautomata.idNodeFather != 0:
			self.currentSubautomata = self.getRootSubautomata()


	def createNewState(self, node, color):
		if self.currentSubautomata.id == 1:
			self.treeModel.insertState(node, color)
		else:
			self.fillTreeView(node, self.treeModel.getChildren(), color)

		node.draw(self.schemaScene)


	def createNewTransition(self, trans):
		#TODO    AUTOTRANSICIONES  
		trans.draw(self.schemaScene)


	def fillTreeView(self, node, children, color):
		added = False
		for child in children:
			if node.getIdNodeFather() == child.id:
				self.treeModel.insertState(node, color, child)
				added = True
			else:
				added = self.fillTreeView(node, child.childItems, color)
			if added:
				break
		return added


	def getSubautomata(self, id):
		for subautomata in self.subautomataList:
			if subautomata.id == id:
				return subautomata
		return None


	def getSubautomataWithNode(self, nodeName):
		for subautomata in self.subautomataList:
			for node in subautomata.nodeList:
				if node.name == nodeName:
					return subautomata
		return None


	def getRootSubautomata(self):
		for subautomata in self.subautomataList:
			if subautomata.idNodeFather == 0:
				return subautomata


	def getNode(self, nodeId):
		for subautomata in self.subautomataList:
			for node in subautomata.nodeList:
				if nodeId == node.id:
					return node

		return None


	def changeCurrentSubautomata(self, id):
		self.currentSubautomata.hide()
		self.currentSubautomata = self.getSubautomata(id)
		self.currentSubautomata.show()


	def upButtonClicked(self):
		fatherId = self.currentSubautomata.idNodeFather 
		if fatherId != 0:
			nodeFather = self.getNode(fatherId)
			self.changeCurrentSubautomata(nodeFather.subautomata.id)
		else:
			print "This subautomata does not have any parent"


	def rowClicked(self, index):
		node = self.getNode(index.internalPointer().id)
		if self.currentSubautomata.id != node.subautomata.id:
			self.changeCurrentSubautomata(node.subautomata.id)


	def lastExpandedIsFather(self, index):
		if not self.lastExpanded.isValid():
			return False

		indexAux = index
		while(indexAux.parent().isValid()):
			indexAux = indexAux.parent()
			if indexAux == self.lastExpanded:
				return True
		return False


	def treeViewAutoFocus(self, index):	
		if not self.treeView.isExpanded(index.parent()):
			if not self.lastExpandedIsFather(index):
				self.treeView.collapse(self.lastExpanded)

			if index.parent().isValid():
				self.treeView.expand(index.parent())
		self.lastExpanded = index.parent()


	def refreshTreeView(self, index):
		self.treeView.dataChanged(self.treeView.rootIndex(), index)


	def setActiveTreeView(self, node, isActive, children):
		finded = False

		for child in children:
			if finded:
				break

			if child.name == node.name:
				index = self.treeModel.indexOf(child)
				if isActive:
					child.setColor("green")
					self.refreshTreeView(index)
					if self.autofocus.isChecked():
						self.treeViewAutoFocus(index)
					
				else:
					self.refreshTreeView(index)
					child.setColor("white")
				finded = True
			
			else:
				finded = self.setActiveTreeView(node, isActive, child.getChildren())



	def setNodeAsActive(self, node, subautomata, isActive):
		if isActive:
			subautomata.setActiveNode(node.name)
			node.setColor("green")
		else:
			node.setColor("blue")
			
		rootChildren = self.treeModel.getChildren()
		rootIndex = self.treeView.rootIndex()
		self.setActiveTreeView(node, isActive, rootChildren)

		if node.idSubautSon != 0:
			subSon = self.getSubautomata(node.idSubautSon)
			lastActiveNode = subSon.getNodeByName(subSon.getActiveNode())
			self.setNodeAsActive(lastActiveNode, subSon, isActive)


	def notifySetNodeAsActiveReceived(self, nodeName):
		subAux = self.getSubautomataWithNode(nodeName)
		nodeAux = subAux.getNodeByName(subAux.getActiveNode())		

		if nodeAux != None:
			self.setNodeAsActive(nodeAux, subAux, False)

		nodeAux = subAux.getNodeByName(nodeName)
		self.setNodeAsActive(nodeAux, subAux, True)


	def notifySetNodeAsActive(self, nodeName):
		self.activeNodeSignal.emit(nodeName)


	def autofocusChanged(self):
		if self.autofocus.isChecked():
			self.treeView.collapseAll()


	def createMenu(self, position):
		index = self.treeView.indexAt(position)
		if not index.isValid():
			return
		self.selectedNodeId = index.internalPointer().id
		menu = QtGui.QMenu()
		action = menu.addAction("Open subautomata")
		action.triggered.connect(self.openSubautomataInNewWindow)
		menu.exec_(self.treeView.viewport().mapToGlobal(position))


	def openSubautomataInNewWindow(self):
		subautomata = self.getNode(self.selectedNodeId).subautomata

		if subautomata:
			newWindow = AdditionalWindow(self, QtGui.QGraphicsScene(), subautomata)
			newWindow.schema.setScene(newWindow.scene)
			title = "Subautomata " + str(subautomata.id)
			newWindow.title.setText(title)
			subautomata.drawCopy(newWindow.scene, newWindow.id)
			self.additionalWindows.append(newWindow)
			self.additionalWindows[-1].show()


	def removeWindow(self, window):
		for windowAux in self.additionalWindows:
			if windowAux.id == window.id:
				break
		window.subautomata.removeCopy(window.id)
		self.additionalWindows.remove(window)
