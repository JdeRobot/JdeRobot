from PyQt4 import QtGui
from PyQt4 import QtCore
import math


#CONST
SQUARE_SIDE = 10
ARROW_SIZE = 7

#TODO: NOMBRE DE TRANSICIONES EN PY Y CPP!


class GuiTransition():
	def __init__(self, orig, dest, midp, id, idOrig, idDest, windowId=0):
		self.orig = orig
		self.dest = dest
		self.midp = midp
		self.id = id
		self.idOrigin = idOrig
		self.idDestiny = idDest
		self.copies = []
		self.windowId = windowId

		leftLine = QtCore.QLineF(midp[0], midp[1], orig[0], orig[1])
		rigthLine = QtCore.QLineF(midp[0], midp[1], dest[0], dest[1])

		rigthLine.setLength(rigthLine.length()-20)
		leftLine.setLength(leftLine.length()-20)

		#ARROW
		angle = math.acos(rigthLine.dx()/rigthLine.length())
		if (rigthLine.dy() >= 0):
			angle = (math.pi*2.0) - angle

		arrowP1 = rigthLine.p2() - QtCore.QPointF(math.sin(angle + math.pi / 3.0) * ARROW_SIZE,
											math.cos(angle + math.pi / 3) * ARROW_SIZE)
		arrowP2 = rigthLine.p2() - QtCore.QPointF(math.sin(angle + math.pi - math.pi / 3.0) * ARROW_SIZE,
                                        	math.cos(angle + math.pi - math.pi / 3.0) * ARROW_SIZE)
		arrow = QtGui.QPolygonF()
		
		for point in [rigthLine.p2(), arrowP1, arrowP2]:
			arrow.append(point)

		#GRAPHIC ITEMS
		self.rigthLine = QtGui.QGraphicsLineItem(rigthLine)
		self.leftLine = QtGui.QGraphicsLineItem(leftLine)
		self.square = QtGui.QGraphicsRectItem(midp[0] - SQUARE_SIDE/2, midp[1] - SQUARE_SIDE/2,
											SQUARE_SIDE, SQUARE_SIDE)
		self.arrow = QtGui.QGraphicsPolygonItem(arrow)
		self.paint(1)


	def show(self):
		self.leftLine.show()
		self.rigthLine.show()
		self.square.show()
		self.arrow.show()


	def hide(self):
		self.leftLine.hide()
		self.rigthLine.hide()
		self.square.hide()
		self.arrow.hide()


	def paint(self, width):
		pen = QtGui.QPen(QtGui.QColor("black"), width)
		brush = QtGui.QBrush(QtCore.Qt.SolidPattern)
		brush.setColor(QtGui.QColor("red"))
		self.rigthLine.setPen(pen)
		self.leftLine.setPen(pen)
		self.square.setPen(pen)
		self.square.setBrush(brush)
		brush.setColor(QtGui.QColor("black"))
		self.arrow.setBrush(brush)		


	def draw(self, view):
		view.addItem(self.leftLine)
		view.addItem(self.rigthLine)
		view.addItem(self.square)
		view.addItem(self.arrow)


	def createCopy(self, windowId):
		transAux = GuiTransition(self.orig, self.dest, self.midp, self.id,
								self.idOrigin, self.idDestiny, windowId)
		self.copies.append(transAux)
		return self.copies[-1]


	def removeCopy(self, windowId):
		for copy in self.copies:
			if copy.windowId == windowId:
				self.copies.remove(copy)
