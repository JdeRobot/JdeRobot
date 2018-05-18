import numpy

DEBUG = 1
ANCHO_IMAGEN = 320
LARGO_IMAGEN = 240
DEGTORAD = 3.1415926535897932 / 180
GREEN_MIN = numpy.array([20, 50, 100],numpy.uint8)#numpy.array([48, 138, 138],numpy.uint8)
GREEN_MAX = numpy.array([90, 235, 210],numpy.uint8)#numpy.array([67, 177, 192],numpy.uint8)

def pixel2optical (p2d):
	aux = p2d.x
	p2d.x = LARGO_IMAGEN-1-p2d.y
	p2d.y = aux
	p2d.h = 1

	return p2d

def getIntersectionZ (p2d, mycamera):
	p3d = Punto3D ()
	res = Punto3D ()
	p2d_ = Punto2D ()

	x = myCamera.position.x
	y = myCamera.position.y
	z = myCamera.position.z

	p2d_ = pixel2optical(p2d)
	result, p3d = backproject(p2d_, myCamera)

	# Check division by zero
	if((p3d.z-z) == 0.0):
		res.h = 0.0
		return

	zfinal = 0. # Quiero que intersecte con el Plano Z = 0

	# Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)
	xfinal = x + (p3d.x - x)*(zfinal - z)/(p3d.z-z)
	yfinal = y + (p3d.y - y)*(zfinal - z)/(p3d.z-z)	

	res.x = xfinal
	res.y = yfinal
	res.z = zfinal
	res.h = 1.0

	return res

def loadPiCamCameraModel ():
	myCamera = PinholeCamera ()
	# -------------------------------------------------------------
	# LOADING MATRICES:
	# -------------------------------------------------------------
	thetaY = 59*DEGTORAD # considerando que la camara (en vertical) está rotada 90º sobre eje Y
	thetaZ = 0*DEGTORAD # considerando que la camara (en vertical) está rotada 90º sobre eje Y
	thetaX = 0*DEGTORAD # considerando que la camara (en vertical) está rotada 90º sobre eje Y

	R_y = numpy.array ([(numpy.cos(thetaY),0,-numpy.sin(thetaY)),(0,1,0),(numpy.sin(thetaY),0,numpy.cos(thetaY))]) # R is a 3x3 rotation matrix
	R_z = numpy.array ([(numpy.cos(thetaZ),-numpy.sin(thetaZ),0),(numpy.sin(thetaZ),numpy.cos(thetaZ),0),(0,0,1)]) # R is a 3x3 rotation matrix
	R_x = numpy.array ([(1,0,0),(0,numpy.cos(thetaX),numpy.sin(thetaX)),(0, -numpy.sin(thetaX),numpy.cos(thetaX))]) # R is a 3x3 rotation matrix

	R_subt = numpy.dot (R_y, R_z)
	R_tot = numpy.dot (R_subt, R_x)

	T = numpy.array ([(1,0,0,0),(0,1,0,0),(0,0,1,-110)]) # T is a 3x4 traslation matrix
	Res = numpy.dot (R_tot,T)
	RT = numpy.append(Res, [[0,0,0,1]], axis=0) # RT is a 4x4 matrix
	K = numpy.array ([(313.89382026,0,117.5728043,0),(0,316.64906146,158.04145907,0),(0,0,1,0)]) # K is a 3x4 matrix
	# -------------------------------------------------------------

	# -------------------------------------------------------------
	# LOADING BOTH CAMERA MODELS JUST TO TEST THEM
	# -------------------------------------------------------------
	# A) PROGEO CAMERA
	# -------------------------------------------------------------
	myCamera.position.x = 0
	myCamera.position.y = 0
	myCamera.position.z = -110
	myCamera.position.h = 1

	# K intrinsec parameters matrix (values got from the PiCamCalibration.py)
	myCamera.k11 = K[0,0]
	myCamera.k12 = K[0,1]
	myCamera.k13 = K[0,2]
	myCamera.k14 = K[0,3]

	myCamera.k21 = K[1,0]
	myCamera.k22 = K[1,1]
	myCamera.k23 = K[1,2]
	myCamera.k24 = K[1,3]

	myCamera.k31 = K[2,0]
	myCamera.k32 = K[2,1]
	myCamera.k33 = K[2,2]
	myCamera.k34 = K[2,3]

	# RT rotation-traslation matrix
	myCamera.rt11 = RT[0,0]
	myCamera.rt12 = RT[0,1]
	myCamera.rt13 = RT[0,2]
	myCamera.rt14 = RT[0,3]

	myCamera.rt21 = RT[1,0]
	myCamera.rt22 = RT[1,1]
	myCamera.rt23 = RT[1,2]
	myCamera.rt24 = RT[1,3]

	myCamera.rt31 = RT[2,0]
	myCamera.rt32 = RT[2,1]
	myCamera.rt33 = RT[2,2]
	myCamera.rt34 = RT[2,3]

	myCamera.rt41 = RT[3,0]
	myCamera.rt42 = RT[3,1]
	myCamera.rt43 = RT[3,2]
	myCamera.rt44 = RT[3,3]

	myCamera.fdistx = K[0,0] #myCamera.k11
	myCamera.fdisty = K[1,1] #myCamera.k22
	myCamera.u0 = K[0,2] #myCamera.k13
	myCamera.v0 = K[1,2] #myCamera.k23
	myCamera.rows = LARGO_IMAGEN
	myCamera.columns = ANCHO_IMAGEN

class Punto2D:
	x = 0
	y = 0
	h = 0

class Punto3D:
	x = 0
	y = 0
	z = 0
	h = 0

class PinholeCamera:
	position = Punto3D() # camera 3d position in mm 
	foa = Punto3D() # camera 3d focus of attention in mm 
	roll = None # camera roll position angle in rads 
	fdistx = None # focus x distance in mm
	fdisty = None # focus y distance in mm
	u0 = None # pixels 
	v0 = None
	skew = None #angle between the x and y pixel axes in rads
	rows = None # image height in pixels 
	columns = None # image width in pixels 
	# camera K matrix 
	k11 = None
	k12 = None
	k13 = None
	k14 = None
	k21 = None
	k22 = None
	k23 = None
	k24 = None
	k31 = None
	k32 = None
	k33 = None
	k34 = None
	# camera rotation + translation matrix 
	rt11 = None
	rt12 = None
	rt13 = None
	rt14 = None
	rt21 = None
	rt22 = None
	rt23 = None
	rt24 = None
	rt31 = None
	rt32 = None
	rt33 = None
	rt34 = None
	rt41 = None
	rt42 = None
	rt43 = None
	rt44 = None
	# distortion parameters 
	d1 = None
	d2 = None
	d3 = None
	d4 = None
	d5 = None
	d6 = None
	dx = None
	dy = None

	def printCameraInfo (self):
		print "------------------------------------------------------"
		print "PROGEO-MODEL CAMERA INFO"
		print "     Position: (X,Y,Z,H)= ",self.position.x,self.position.y,self.position.z,self.position.h
		print "     Focus of Attention: (x,y,z,h)= ",self.foa.x,self.foa.y,self.foa.z,self.foa.h
		print "     Focus DistanceX(vertical): ",self.fdistx
		print "     Focus DistanceY(horizontal): ",self.fdisty
		print "     Skew: ",self.skew
		print "     Optical Center: (u0,v0)= ",self.u0,self.v0
		print "     K Matrix: ",self.k11,self.k12,self.k13,self.k14
		print "               ",self.k21,self.k22,self.k23,self.k24
		print "               ",self.k31,self.k32,self.k33,self.k34
		print "     RT Matrix: ",self.rt11,self.rt12,self.rt13,self.rt14
		print "                ",self.rt21,self.rt22,self.rt23,self.rt24
		print "                ",self.rt31,self.rt32,self.rt33,self.rt34
		print "                ",self.rt41,self.rt42,self.rt43,self.rt44
		print "------------------------------------------------------\n"

def project (punto3D, camera):
	punto2D = Punto2D()

	# returns -1 if the point lies behind the camera, returns 1 if "punto3D" 3Dpoint projects into a 2D finite point, 0 otherwise
	a1 = camera.rt11*punto3D.x+camera.rt12*punto3D.y+camera.rt13*punto3D.z+camera.rt14*punto3D.h
	a2 = camera.rt21*punto3D.x+camera.rt22*punto3D.y+camera.rt23*punto3D.z+camera.rt24*punto3D.h
	a3 = camera.rt31*punto3D.x+camera.rt32*punto3D.y+camera.rt33*punto3D.z+camera.rt34*punto3D.h
	a4 = camera.rt41*punto3D.x+camera.rt42*punto3D.y+camera.rt43*punto3D.z+camera.rt44*punto3D.h

	punto2D.x=camera.k11*a1+camera.k12*a2+camera.k13*a3
	punto2D.y=camera.k21*a1+camera.k22*a2+camera.k23*a3
	punto2D.h=camera.k31*a1+camera.k32*a2+camera.k33*a3

	if (punto2D.h!=0.):
		punto2D.x=punto2D.x/punto2D.h
		punto2D.y=punto2D.y/punto2D.h
		punto2D.h=1.

		# a3/a4 is the number whose sign is interesting, but it has the same sign as a3*a4 and avoids to divide
		if (a3*a4<=0.):
			punto2D.h=-1.
			output=-1 # point behind the focal plane 
		else:
			output=1
	else:
		output=0

	return (output, punto2D)

def backproject (punto2D, camera):
	output = -1
	temp2D = Punto2D()
	punto3D = Punto3D()

	if (punto2D.h>0.):
		temp2D.h=camera.k11
		temp2D.x=punto2D.x*camera.k11/punto2D.h
		temp2D.y=punto2D.y*camera.k11/punto2D.h

		ik11=(1./camera.k11)
		ik12=-camera.k12/(camera.k11*camera.k22)
		ik13=(camera.k12*camera.k23-camera.k13*camera.k22)/(camera.k22*camera.k11)
		ik21=0.
		ik22=(1./camera.k22)
		ik23=-(camera.k23/camera.k22)
		ik31=0.
		ik32=0.
		ik33=1.

		a1=ik11*temp2D.x+ik12*temp2D.y+ik13*temp2D.h
		a2=ik21*temp2D.x+ik22*temp2D.y+ik23*temp2D.h
		a3=ik31*temp2D.x+ik32*temp2D.y+ik33*temp2D.h
		a4=1.

		ir11=camera.rt11
		ir12=camera.rt21
		ir13=camera.rt31
		ir14=0.
		ir21=camera.rt12
		ir22=camera.rt22
		ir23=camera.rt32
		ir24=0.
		ir31=camera.rt13
		ir32=camera.rt23
		ir33=camera.rt33
		ir34=0.
		ir41=0.
		ir42=0.
		ir43=0.
		ir44=1.

		b1=ir11*a1+ir12*a2+ir13*a3+ir14*a4
		b2=ir21*a1+ir22*a2+ir23*a3+ir24*a4
		b3=ir31*a1+ir32*a2+ir33*a3+ir34*a4
		b4=ir41*a1+ir42*a2+ir43*a3+ir44*a4 

		it11=1.
		it12=0.
		it13=0.
		it14=camera.position.x
		it21=0.
		it22=1.
		it23=0.
		it24=camera.position.y
		it31=0.
		it32=0.
		it33=1.
		it34=camera.position.z
		it41=0.
		it42=0.
		it43=0.
		it44=1.

		punto3D.x=it11*b1+it12*b2+it13*b3+it14*b4
		punto3D.y=it21*b1+it22*b2+it23*b3+it24*b4
		punto3D.z=it31*b1+it32*b2+it33*b3+it34*b4
		punto3D.h=it41*b1+it42*b2+it43*b3+it44*b4

		if (punto3D.h!=0.):
			punto3D.x=punto3D.x/punto3D.h
			punto3D.y=punto3D.y/punto3D.h
			punto3D.z=punto3D.z/punto3D.h
			punto3D.h=1.
			output=1
		else:
			output=0

	return(output, punto3D)

# if p1 and p2 can't be drawed in a camera.cols X camera.rows image, then it will return 0. Otherwise it will return 1 and gooda & goodb will be the correct points in the image to be drawn
# p1 and p2 are both 2dpoints, output are a and b, 2dpoints also
def displayline (p1, p2, camera):
# it takes care of important features: before/behind the focal plane, inside/outside the image frame
	mycase=0
	papb=0.

	Xmin=0.
	Xmax=camera.rows-1.
	Ymin=0.
	Ymax=camera.columns-1.
	# Watchout!: they can't reach camera.rows or camera.columns, their are not valid values for the pixels 

	l0.x=0.
	l0.y=1.
	l0.h=-Ymin
	l1.x=0.
	l1.y=1.
	l1.h=-Ymax
	l2.x=1.
	l2.y=0.
	l2.h=-Xmax
	l3.x=1.
	l3.y=0.
	l3.h=-Xmin

	if ((p1.h<0.)and(p2.h<0.)):
		# both points behind the focal plane: don't display anything 
		mycase=10
	else:
		if ((p1.h>0.)and(p2.h<0.)):
			# p1 before the focal plane, p2 behind 
			#Calculates p2 = p1 + -inf(p2-p1)
			p2.x = p1.x + (-BIGNUM)*(p2.x-p1.x)
			p2.y = p1.y + (-BIGNUM)*(p2.y-p1.y)
			p2.h=-p2.h # undo the "project" trick to get the right value 
		elif ((p1.h<0.)and(p2.h>0.)):
			# p2 before the focal plane, p1 behind 
			#Calculates p1 = p2 + -inf(p1-p2)
			p1.x = p2.x + (-BIGNUM)*(p1.x-p2.x)
			p1.y = p2.y + (-BIGNUM)*(p1.y-p2.y)
			p1.h=-p1.h # undo the "project" trick to get the right value 

		# both points before the focal plane 
		if ((p1.x>=Xmin) and (p1.x<Xmax+1) and (p1.y>=Ymin) and (p1.y<Ymax+1) and	(p2.x>=Xmin) and (p2.x<Xmax+1) and (p2.y>=Ymin) and (p2.y<Ymax+1)):
			# both inside the image limits 
			gooda.x=p1.x
			gooda.y=p1.y
			gooda.h=p1.h
			goodb.x=p2.x
			goodb.y=p2.y
			goodb.h=p2.h
			mycase=2
		elif ((p1.x>=Xmin) and (p1.x<Xmax+1) and (p1.y>=Ymin) and (p1.y<Ymax+1) and ((p2.x<Xmin) or (p2.x>=Xmax+1) or (p2.y<Ymin) or (p2.y>=Ymax+1))):
			# p1 inside, p2 outside 
			gooda.x=p1.x
			gooda.y=p1.y
			gooda.h=p1.h
			goodb.x=p1.x
			goodb.y=p1.y
			goodb.h=p1.h
			pa.x=p1.x
			pa.y=p1.y
			pa.h=p1.h
			pb.x=p2.x
			pb.y=p2.y
			pb.h=p2.h
			mycase=3
		elif ((p2.x>=Xmin) and (p2.x<Xmax+1) and (p2.y>=Ymin) and (p2.y<Ymax+1) and ((p1.x<Xmin) or (p1.x>=Xmax+1) or (p1.y<Ymin) or (p1.y>=Ymax+1))):
			# p2 inside, p1 outside 
			gooda.x=p2.x
			gooda.y=p2.y
			gooda.h=p2.h
			goodb.x=p2.x
			goodb.y=p2.y
			goodb.h=p2.h
			pa.x=p2.x
			pa.y=p2.y
			pa.h=p2.h
			pb.x=p1.x
			pb.y=p1.y
			pb.h=p1.h
			mycase=4
		else:
			# both outside     
			pa.x=p2.x
			pa.y=p2.y
			pa.h=p2.h
			pb.x=p1.x
			pb.y=p1.y
			pb.h=p1.h
			mycase=5

		l.x=pa.y*pb.h-pb.y*pa.h
		l.y=pb.x*pa.h-pa.x*pb.h
		l.h=pa.x*pb.y-pb.x*pa.y
		i0.x=l.y*l0.h-l.h*l0.y
		i0.y=l.h*l0.x-l.x*l0.h
		i0.h=l.x*l0.y-l.y*l0.x
		i1.x=l.y*l1.h-l.h*l1.y
		i1.y=l.h*l1.x-l.x*l1.h
		i1.h=l.x*l1.y-l.y*l1.x
		i2.x=l.y*l2.h-l.h*l2.y
		i2.y=l.h*l2.x-l.x*l2.h
		i2.h=l.x*l2.y-l.y*l2.x
		i3.x=l.y*l3.h-l.h*l3.y
		i3.y=l.h*l3.x-l.x*l3.h
		i3.h=l.x*l3.y-l.y*l3.x

		if (i0.h!=0.):
			i0.x=i0.x/i0.h
			i0.y=i0.y/i0.h
			i0.h=1.
		if (i1.h!=0.):
			i1.x=i1.x/i1.h
			i1.y=i1.y/i1.h
			i1.h=1.
		if (i2.h!=0.):
			i2.x=i2.x/i2.h
			i2.y=i2.y/i2.h
			i2.h=1.
		if (i3.h!=0.):
			i3.x=i3.x/i3.h
			i3.y=i3.y/i3.h
			i3.h=1.

		papb=(pb.x-pa.x)*(pb.x-pa.x)+(pb.y-pa.y)*(pb.y-pa.y)
		maxdot = -1

		if (i0.h!=0.):
			if ((i0.x>=Xmin) and (i0.x<Xmax+1) and (i0.y>=Ymin) and (i0.y<Ymax+1)):
				if ((((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))>=0.) and (((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))<papb) and (((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))>maxdot)):
					if ((mycase==3)or(mycase==4)or(mycase==6)):
						goodb.x=i0.x
						goodb.y=i0.y
						goodb.h=i0.h
						maxdot = (pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y)
					elif (mycase==5):
						gooda.x=i0.x
						gooda.y=i0.y
						gooda.h=i0.h
						goodb.x=i0.x
						goodb.y=i0.y
						goodb.h=i0.h
						mycase=6
		# else i0 at infinite, parallel lines 

		if (i1.h!=0.): 
			if ((i1.x>=Xmin) and (i1.x<Xmax+1) and (i1.y>=Ymin) and (i1.y<Ymax+1)):
				if ((((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))>=0.)and (((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))<papb) and	(((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))>maxdot)):
					if ((mycase==3)or(mycase==4)or(mycase==6)):
						goodb.x=i1.x
						goodb.y=i1.y
						goodb.h=i1.h
						maxdot = (pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y)
					elif (mycase==5):
						gooda.x=i1.x
						gooda.y=i1.y
						gooda.h=i1.h
						goodb.x=i1.x
						goodb.y=i1.y
						goodb.h=i1.h
						mycase=6
		# else i0 at infinite, parallel lines  # i1 at infinite, parallel lines 

		if (i2.h!=0.):
			if ((i2.x>=Xmin) and (i2.x<Xmax+1) and (i2.y>=Ymin) and (i2.y<Ymax+1)):
				if ((((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))>=0.)and	(((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))<papb) and	(((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))>maxdot)):
					if ((mycase==3)or(mycase==4)or(mycase==6)):
						goodb.x=i2.x
						goodb.y=i2.y
						goodb.h=i2.h
						maxdot = (pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y)
					elif (mycase==5):
						gooda.x=i2.x
						gooda.y=i2.y
						gooda.h=i2.h
						goodb.x=i2.x
						goodb.y=i2.y
						goodb.h=i2.h
						mycase=6
		# else i0 at infinite, parallel lines  # i2 at infinite, parallel lines 

		if (i3.h!=0.):
			if ((i3.x>=Xmin) and (i3.x<Xmax+1) and (i3.y>=Ymin) and (i3.y<Ymax+1)):
				if ((((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))>=0.) and (((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))<papb) and (((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))>maxdot)):
					if ((mycase==3)or(mycase==4)or(mycase==6)):
						goodb.x=i3.x
						goodb.y=i3.y
						goodb.h=i3.h
						maxdot = (pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y)
					elif (mycase==5):
						gooda.x=i3.x
						gooda.y=i3.y
						gooda.h=i3.h
						goodb.x=i3.x
						goodb.y=i3.y
						goodb.h=i3.h
						mycase=6
		# else i0 at infinite, parallel lines  # i3 at infinite, parallel lines 

	if (DEBUG==1):
		print("p3: x=",p1.x," y=",p1.y," h=",p1.h,"\np2: x=",p2.x,", y=",p2.y," h=",p2.h,"\n")
		print("case: ",mycase,"\n i0: x=",i0.x," y=",i0.y," h=",i0.h," dot=",((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y)),"\n i1: x=",i1.x," y=",i1.y," h=",i1.h," dot=",((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y)),"\n i2: x=",i2.x," y=",i2.y," h=",i2.h," dot=",((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y)),"\n i3: x=",i3.x," y=",i3.y," z=",i3.h," dot=",((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y)),"\n")
		print("gooda:  x=",gooda.x," y=",gooda.y," z=",gooda.h,"\n")
		print("goodb:  x=",goodb.x," y=",goodb.y," z=",goodb.h,"\n")

	a.x=gooda.x
	b.x=goodb.x
	a.y=gooda.y
	b.y=goodb.y
	a.h=gooda.h
	b.h=goodb.h

	if((mycase!=2)and(mycase!=3)and(mycase!=4)and(mycase!=6)):
		output = 0
	else:
		output = 1

	return (output, a, b)

