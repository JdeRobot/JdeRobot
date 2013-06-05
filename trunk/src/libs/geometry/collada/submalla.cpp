#include "submalla.h"

SubMalla::SubMalla()
{
    materialIndex = -1;
}

SubMalla::~SubMalla()
{
}

//////////////////////////////////////////////////
void SubMalla::setName(const std::string &_n)
{
  this->name = _n;
}

//////////////////////////////////////////////////
void SubMalla::setPrimitiveType(PrimitiveType _type)
{
  this->primitiveType = _type;
}

//////////////////////////////////////////////////
int SubMalla::getPrimitiveType() const
{
  return this->primitiveType;
}

//////////////////////////////////////////////////
void SubMalla::addVertex(const math::Vector3 &_v)
{
  this->vertices.push_back(_v);
}

//////////////////////////////////////////////////
void SubMalla::addIndex(unsigned int _i)
{
  this->indices.push_back(_i);
}

//////////////////////////////////////////////////
unsigned int SubMalla::getVertexCount() const
{
  return this->vertices.size();
}

//////////////////////////////////////////////////
math::Vector3 SubMalla::getVertex(unsigned int _i) const
{
  if (_i >= this->vertices.size())
    std::cout << "Index too large\n";

  return this->vertices[_i];
}

//////////////////////////////////////////////////
math::Vector3 SubMalla::getNormal(unsigned int _i) const
{
  if (_i >= this->normals.size())
    std::cout <<"Index too large";

  return this->normals[_i];
}

//////////////////////////////////////////////////
unsigned int SubMalla::getNormalCount() const
{
  return this->normals.size();
}

//////////////////////////////////////////////////
void SubMalla::setMaterialIndex(unsigned int _index)
{
  this->materialIndex = _index;
}

//////////////////////////////////////////////////
unsigned int SubMalla::getMaterialIndex() const
{
  return this->materialIndex;
}

//////////////////////////////////////////////////
void SubMalla::addNormal(const math::Vector3 &_n)
{
  this->normals.push_back(_n);
}

//////////////////////////////////////////////////
void SubMalla::addTexCoord(double _u, double _v)
{
  this->texCoords.push_back(math::Vector2d(_u, _v));
}

//////////////////////////////////////////////////
unsigned int SubMalla::getTexCoordCount() const
{
  return this->texCoords.size();
}

//////////////////////////////////////////////////
unsigned int SubMalla::getIndexCount() const
{
  return this->indices.size();
}


//////////////////////////////////////////////////
unsigned int SubMalla::getIndex(unsigned int _i) const
{
  if (_i > this->indices.size())
    std::cout << "Index too large";

  return this->indices[_i];
}

//////////////////////////////////////////////////
math::Vector2d SubMalla::getTexCoord(unsigned int _i) const
{
  if (_i >= this->texCoords.size())
    std::cout << "Index too large";

  return this->texCoords[_i];
}

//////////////////////////////////////////////////
void SubMalla::center()
{
 math::Vector3 _center(0, 0, 0);

  math::Vector3 min, max, half;
  min = this->getMin();
  max = this->getMax();
  half = (max - min) * 0.5;

  this->translate(_center - (min + half));
}

//////////////////////////////////////////////////
void SubMalla::translate(const math::Vector3 &_vec)
{
  for (std::vector<math::Vector3>::iterator iter = this->vertices.begin();
       iter != this->vertices.end(); ++iter)
  {
    (*iter) += _vec;
  }
}

//////////////////////////////////////////////////
math::Vector3 SubMalla::getMax() const
{
  math::Vector3 max;
  std::vector<math::Vector3>::const_iterator iter;

  max.setX( -std::numeric_limits<float>::max());
  max.setY( -std::numeric_limits<float>::max());
  max.setZ( -std::numeric_limits<float>::max());

  for (iter = this->vertices.begin(); iter != this->vertices.end(); ++iter)
  {
    math::Vector3 v = (*iter);
    max.setX(std::max(max.getX(), v.getX()));
    max.setY(std::max(max.getY(), v.getY()));
    max.setZ(std::max(max.getZ(), v.getZ()));
  }

  return max;
}

//////////////////////////////////////////////////
math::Vector3 SubMalla::getMin() const
{
  math::Vector3 min;
  std::vector<math::Vector3>::const_iterator iter;

  min.setX( std::numeric_limits<float>::max());
  min.setY( std::numeric_limits<float>::max());
  min.setZ( std::numeric_limits<float>::max());

  for (iter = this->vertices.begin(); iter != this->vertices.end(); ++iter)
  {
    math::Vector3 v = (*iter);
    min.setX(std::min(min.getX(), v.getX()));
    min.setY(std::min(min.getY(), v.getY()));
    min.setZ(std::min(min.getZ(), v.getZ()));

  }

  return min;
}

//////////////////////////////////////////////////
// PAra ODE
////
void SubMalla::FillArrays(float **_vertArr, int **_indArr)
{
  if (this->vertices.size() == 0 || this->indices.size() == 0)
    std::cout << "No vertices or indices\n";

  std::vector< math::Vector3 >::const_iterator viter;
  std::vector< unsigned int >::const_iterator iiter;
  unsigned int i;

  if (*_vertArr)
    delete [] *_vertArr;

  if (*_indArr)
    delete [] *_indArr;

  *_vertArr = new float[this->vertices.size() * 3];
  *_indArr = new int[this->indices.size()];

  for (viter = this->vertices.begin(), i = 0; viter != this->vertices.end();
      ++viter){
      (*_vertArr)[i++] = static_cast<float>((*viter).vector(0));
      (*_vertArr)[i++] = static_cast<float>((*viter).vector(1));
      (*_vertArr)[i++] = static_cast<float>((*viter).vector(2));
  }

  for (iiter = this->indices.begin(), i = 0;
      iiter != this->indices.end(); ++iiter)
    (*_indArr)[i++] = (*iiter);
}




