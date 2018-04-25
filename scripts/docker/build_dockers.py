import docker
import yaml
import sys


level = 0

yamlFile = sys.argv[1]
client = docker.from_env()
images={}


## Build Image ##
def build_image(ipath, itag):
  try:
    client.images.build(pull=True, path=ipath, tag=itag, rm=True, stream=True)
  except docker.errors.BuildError as exc:
    print(exc)
  except docker.errors.APIError as exc:
    print (exc)

## Push Image ##    
def push_image(itag):
  try:
    client.images.push(itag)
  except docker.errors.APIError as exc:
    print (exc)




## Open File ##
with open(yamlFile, 'r') as stream:
  try:
    images=yaml.load(stream)
  except yaml.YAMLError as exc:
    print(exc)

while images:
  imgs = dict(images)
  for image in images:
    if images[image]["level"] == level:
      if images[image]["build"]:
        print "Building " + image 
        build_image(images[image]["path"], images[image]["tag"])
      if images[image]["push"]:
        print "Pushing " + image 
        push_image(images[image]["tag"])
      del imgs[image]
  images = dict(imgs)
  level = level + 1
  
  
  
  
