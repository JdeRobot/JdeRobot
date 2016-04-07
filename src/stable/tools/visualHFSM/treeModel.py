from PyQt4 import QtCore, QtGui
import threading


class TreeNode(QtGui.QTreeWidgetItem):
	def __init__(self, id, name, color, parent=None):
		QtGui.QTreeWidgetItem.__init__(self)

		self.parentItem = parent
		self.id = id
		self.name = name
		self.color = color
		self.childItems = []


	def background(self):
		background = QtCore.QVariant(QtGui.QColor(self.color))
		return background

	def appendChild(self, item):
		self.childItems.append(item)

	def child(self, row):
		return self.childItems[row]

	def childCount(self):
		return len(self.childItems)

	def columnCount(self):
		return 2

	def data(self, column):
		if column == 0:
			return self.id
		if column == 1:
			return self.name

	def parent(self):
		return self.parentItem

	def row(self):
		if self.parentItem:
			return self.parentItem.childItems.index(self)
		return 0

	def getChildren(self):
		return self.childItems

	def setColor(self, color):
		self.color = color
		

class TreeModel(QtCore.QAbstractItemModel):
	def __init__(self, parent=None):
		super(TreeModel, self).__init__(parent)
		self.rootNode = TreeNode("ID", "Name", "white")

	def columnCount(self, parent):
		if parent.isValid():
			return parent.internalPointer().columnCount()
		else:
			return self.rootNode.columnCount()

	def data(self, index, role):
		if not index.isValid():
			return None

		if role == QtCore.Qt.DisplayRole:
			item = index.internalPointer()
			return item.data(index.column())

		elif role == QtCore.Qt.BackgroundColorRole:
			item = index.internalPointer()
			return item.background()

		
		return None


	def rowId(self, index):
		if not index.isValid():
			return 0

		if not index.parent().isValid():
			item = index.internalPointer()
			return item.id
		else:
			return 0

	def flags(self, index):
		if not index.isValid():
			return QtCore.Qt.NoItemFlags

		return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

	def headerData(self, section, orientation, role):
		if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
			return self.rootNode.data(section)

		return None

	def index(self, row, column, parent):
		if not self.hasIndex(row, column, parent):
			return QtCore.QModelIndex()

		if not parent.isValid():
			parentItem = self.rootNode
		else:
			parentItem = parent.internalPointer()

		childItem = parentItem.child(row)
		if childItem:
			return self.createIndex(row, column, childItem)
		else:
			return QtCore.QModelIndex()

	def indexOf(self, child):
		return self.createIndex(child.row(), 0, child)

	def parent(self, index):
		if not index.isValid():
			return QtCore.QModelIndex()

		childItem = index.internalPointer()
		parentItem = childItem.parent()

		if parentItem == self.rootNode:
			return QtCore.QModelIndex()

		return self.createIndex(parentItem.row(), 0, parentItem)


	def rowCount(self, parent):
		if parent.column() > 0:
			return 0

		if not parent.isValid():
			parentItem = self.rootNode
		else:
			parentItem = parent.internalPointer()

		return parentItem.childCount()


	def insertState(self, state, color, parent=None):
		if parent is None:
			parent = self.rootNode
		parent.appendChild(TreeNode(state.id, state.name, color, parent))


	def getChildren(self):
		return self.rootNode.childItems