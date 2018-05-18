import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel,QMainWindow
from PyQt5.QtGui import QIcon, QPixmap,QPainter, QColor, QPen,QMouseEvent
from PyQt5.Qt import Qt
from PyQt5 import QtCore
from PyQt5.QtCore import QSize, pyqtSignal
import numpy as np
from math import sin,cos,radians,sqrt,degrees,atan,acos
import warnings
warnings.filterwarnings("error")
class HsvWidget(QWidget):
	
	DELTA = 300
	hue_min=1
   	hue_max=359
   	sat_min=1
   	sat_max=255
 	hsvUpdate=pyqtSignal()
 	guiUpdate=pyqtSignal()
 	sliderUpdate=pyqtSignal(int,int,int,int)
 	def __init__(self,winParent):      
	    super(HsvWidget, self).__init__()
	    self.winParent=winParent
	    self.hsvUpdate.connect(self.updateHsv)
	    self.guiUpdate.connect(self.updategui)
	    self.sliderUpdate.connect(self.winParent.updateSlider)
	    self.title = 'HSV colour disc'
	    self.left = 1500
	    self.top = 10
	    self.width = 510
	    self.height = 510
	    self.center = None
	    self.disc_center=255
	    self.points = np.array([[(self.sat_min*cos(radians(self.hue_max)))+self.disc_center,self.disc_center-(self.sat_min*sin(radians(self.hue_max)))],
	                            [(self.sat_min*cos(radians(self.hue_min)))+self.disc_center,self.disc_center-(self.sat_min*sin(radians(self.hue_min)))],
	                            [(self.sat_max*cos(radians(self.hue_min)))+self.disc_center,self.disc_center-(self.sat_max*sin(radians(self.hue_min)))],
	                            [(self.sat_max*cos(radians(self.hue_max)))+self.disc_center,self.disc_center-(self.sat_max*sin(radians(self.hue_max)))]],dtype=np.float)
	    self.draggin_idx = -1
	    self.initUI()
	def initUI(self):
	    self.setMinimumSize(511,511)
	    self.setMaximumSize(511,511)
	    self.setWindowTitle(self.title)
	    self.setGeometry(QtCore.QRect(self.left, self.top, self.width, self.height))
	    self.hsvLabel=QLabel(self)
	    self.hsvLabel.show()

	def updateHsv(self):
	    self.hue_min=self.winParent.hsvdwn[0]
	    self.hue_max=self.winParent.hsvup[0]
	    self.sat_min=self.winParent.hsvdwn[1]
	    self.sat_max=self.winParent.hsvup[1]
	    #print(self.hue_min)
	    #print(self.hue_max)
	    self.points = np.array([[(self.sat_min*cos(radians(self.hue_max)))+self.disc_center,self.disc_center-(self.sat_min*sin(radians(self.hue_max)))],
                                [(self.sat_min*cos(radians(self.hue_min)))+self.disc_center,self.disc_center-(self.sat_min*sin(radians(self.hue_min)))],
                                [(self.sat_max*cos(radians(self.hue_min)))+self.disc_center,self.disc_center-(self.sat_max*sin(radians(self.hue_min)))],
                                [(self.sat_max*cos(radians(self.hue_max)))+self.disc_center,self.disc_center-(self.sat_max*sin(radians(self.hue_max)))]],dtype=np.float)
	    self.guiUpdate.emit()
	    #self.update()
	def updategui(self):
		self.paintEvent()
	def _get_point(self, event):
	    return np.array([event.pos().x(),event.pos().y()])

	def mousePressEvent(self, event):
	    
	    if event.button() == QtCore.Qt.LeftButton and self.draggin_idx == -1:
	        point = self._get_point(event)
	        #print(point)
	        event = QMouseEvent(QtCore.QEvent.MouseButtonPress, event.pos(), QtCore.Qt.LeftButton, QtCore.Qt.LeftButton, QtCore.Qt.NoModifier)
	        self.center = event.pos()
	        dist = self.points - point
	        #print('dist=',dist)
	        dist = dist[:,0]**2 + dist[:,1]**2
	        dist[dist>self.DELTA] = np.inf #obviate the distances above DELTA
	        if dist.min() < np.inf:
	            self.draggin_idx = dist.argmin()
	        #print(self.center.x(),self.center.y())

	        self.update()


	def mouseMoveEvent(self, event):
	    #self.center = event.pos()
	    #self.update()
	    if self.draggin_idx != -1:
	    	#print('move')
	        point = self._get_point(event)
	        #print('dragging id=',self.draggin_idx)
	        self.points[self.draggin_idx] = point
	        self.points[3-self.draggin_idx]=self.change_point_type_1(self.points[self.draggin_idx],self.points[3-self.draggin_idx])
	        if self.draggin_idx==0:
	            self.point_to_be_changed=1
	        elif self.draggin_idx==1:
	            self.point_to_be_changed=0
	        elif self.draggin_idx==2:
	            self.point_to_be_changed=3
	        elif self.draggin_idx==3:
	            self.point_to_be_changed=2
	        self.points[self.point_to_be_changed]=self.change_point_type_2(self.points[self.draggin_idx],self.points[self.point_to_be_changed])
	        #print("dragging point=",self.points[self.draggin_idx])
	        #print("changed point",self.points[self.point_to_be_changed])
	        if self.draggin_idx==1 or self.draggin_idx==2:
	            if self.points[self.draggin_idx][1]>self.disc_center:
	                self.hue_min=-degrees(acos((self.points[self.draggin_idx][0]-self.disc_center)/self.distance_from_center(self.points[self.draggin_idx])))
	            else:
	                self.hue_min=degrees(acos((self.points[self.draggin_idx][0]-self.disc_center)/self.distance_from_center(self.points[self.draggin_idx])))
	        elif self.draggin_idx==0 or self.draggin_idx==3:
	            if self.points[self.draggin_idx][1]>self.disc_center:
	                self.hue_max=-degrees(acos((self.points[self.draggin_idx][0]-self.disc_center)/self.distance_from_center(self.points[self.draggin_idx])))
	            else:
	                self.hue_max=degrees(acos((self.points[self.draggin_idx][0]-self.disc_center)/self.distance_from_center(self.points[self.draggin_idx])))
	        #print('AFTER temp_min1=',temp_min1,'temp_min2=',temp_min2,'hue_min',self.hue_min)
	        if self.hue_min<0:
	            self.hue_min=180+abs(-180-self.hue_min)
	        if self.hue_max<0:
	            self.hue_max=180+abs(-180-self.hue_max)
	        if self.hue_max<self.hue_min:
	            temp=self.hue_min
	            self.hue_min=self.hue_max
	            self.hue_max=temp
	        if self.draggin_idx==0 or self.draggin_idx==1:
	        	self.sat_min=sqrt((self.points[self.draggin_idx][0]-self.disc_center)**2+(self.points[self.draggin_idx][1]-self.disc_center)**2)
	        elif self.draggin_idx==2 or self.draggin_idx==3:	
	        	self.sat_max=sqrt((self.points[self.draggin_idx][0]-self.disc_center)**2+(self.points[self.draggin_idx][1]-self.disc_center)**2)
	        
	        self.sliderUpdate.emit(int(self.hue_min),int(self.hue_max),int(self.sat_min),int(self.sat_max))
	        #print(self.sat_min,self.sat_max)
	        self.update()
	def mouseReleaseEvent(self, event):
	    if event.button() == QtCore.Qt.LeftButton and self.draggin_idx != -1:
	        point = self._get_point(event)
	        self.points[self.draggin_idx] = point
	        self.draggin_idx = -1
	        self.update()
	def paintEvent(self,event=None):
	    
	    painter = QPainter(self)
	    pixmap=QPixmap(":/images/wheelfinal.png")

	    #painter.begin(self)
	    #painter.drawPixmap(QtCore.QRect(self.left, self.top, self.width, self.height), self.pixmap)
	    painter.drawPixmap(self.rect(), pixmap)
	    self.resize(pixmap.width(),pixmap.height())
	    
	    pen = QPen(Qt.red, 3)
	    painter.setPen(pen)
	    
	    painter.drawArc (self.disc_center-int(self.distance_from_center(self.points[0])) , 
	                        self.disc_center - int(self.distance_from_center (self.points[0])) , 
	                        2*int(self.distance_from_center(self.points[0])) , 
	                        2*int(self.distance_from_center(self.points[0])) , 
	                        int(16*self.hue_min) , 
	                        int(16*(self.hue_max-self.hue_min)))#arc joining point 0 and point 1
	    painter.drawLine(int(self.points[0][0]),int(self.points[0][1]),int(self.points[3][0]),int(self.points[3][1]))#line joining point 0 and point 3
	    painter.drawArc(self.disc_center-int(self.distance_from_center(self.points[2])) , 
	                        self.disc_center - int(self.distance_from_center (self.points[2])) , 
	                        2*int(self.distance_from_center(self.points[2])) , 
	                        2*int(self.distance_from_center(self.points[2])) , 
	                        int(16*self.hue_min) , 
	                        int(16*(self.hue_max-self.hue_min)))#arc joining point 2 and 3
	    painter.drawLine(int(self.points[1][0]),int(self.points[1][1]),int(self.points[2][0]),int(self.points[2][1]))#line joining point 1 and 2
	    
	    self.update()
	def distance_from_center(self,point):
	        x=sqrt((point[0]-self.disc_center)**2+(point[1]-self.disc_center)**2)
	        return x

	'''Change point along the line'''
	def change_point_type_1(self,s,ch):  
	        r=((ch[0]-self.disc_center)**2)+((ch[1]-self.disc_center)**2)
	        try:
	            k=(s[1]-self.disc_center)/(s[0]-self.disc_center)
	            self.prev_k=k
	        except Warning:
	            k=self.prev_k

	        a=1+(k**2)
	        b=-(2*self.disc_center)-(2*(k**2)*self.disc_center)
	        c=(k**2)*(self.disc_center**2)-r+(self.disc_center**2)
	        cfx,cfy=self.quadsolve(a,b,c,k,s[1])
	        #cfy=k*(cfx-self.disc_center)+self.disc_center
	        x=np.array([cfx,cfy],dtype=np.float)
	        return (x)
	def quadsolve(self,a,b,c,k,s_y):
	    d=sqrt((b**2)-(4*a*c))
	    cfx=(-b-d)/(2*a)
	    cfy=k*(cfx-self.disc_center)+self.disc_center
	    if cfx>=0 and cfx<=512 and cfy>=0 and cfy<=512 and (s_y-self.disc_center)*(cfy-self.disc_center)>0:
	        return(cfx,cfy)
	    else:
	        cfx=(-b+d)/(2*a)
	        cfy=k*(cfx-self.disc_center)+self.disc_center
	        return (cfx,cfy)
	
	''' Change point along the arc'''
	def change_point_type_2(self,s,ch): 
	    r=((s[0]-self.disc_center)**2)+((s[1]-self.disc_center)**2)
	    try:
	        k=(ch[1]-self.disc_center)/(ch[0]-self.disc_center)
	        self.prev_l=k
	    except Warning:
	        k=self.prev_l
	    cfx=self.disc_center-sqrt((r/(1+(k**2))))
	    cfy=((cfx-self.disc_center)*k)+self.disc_center
	    if cfx>=0 and cfx<=512 and cfy>=0 and cfy<=512 and (ch[0]-self.disc_center)*(cfx-self.disc_center)>0 and (ch[1]-self.disc_center)*(cfy-self.disc_center)>0 :
	        x=np.array([cfx,cfy],dtype=np.float)
	        return(x)
	    else:
	        cfx=self.disc_center+sqrt((r/(1+(k**2))))
	        cfy=((cfx-self.disc_center)*k)+self.disc_center
	        x=np.array([cfx,cfy],dtype=np.float)
	        return(x)
	def closeEvent(self, event):
	    self.winParent.closeHSVWidget()