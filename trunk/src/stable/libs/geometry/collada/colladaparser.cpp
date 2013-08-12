#include "colladaparser.h"
namespace files_3D {

    ColladaParser::ColladaParser(std::string filename, bool to2D, double scalemap, double scale)
    {
        TiXmlDocument xmlDoc;

        this->filename = filename;

        std::cout << "Loading " << filename << std::endl;

        if (!xmlDoc.LoadFile(this->filename)){
            std::cout << "Imposible to load collada file" << std::endl;
            return;
        }
        this->colladaXml = xmlDoc.FirstChildElement("COLLADA");
        if (!this->colladaXml){
            std::cout << "Falta la etiqueta Collada" << std::endl;
            return;
        }
        if (std::string(this->colladaXml->Attribute("version")) != "1.4.0" &&
            std::string(this->colladaXml->Attribute("version")) != "1.4.1"){
            std::cout << "Invalid version. Must be version 1.4.0 or 1.4.1" << std::endl;
            return;

        }
        TiXmlElement *assetXml = this->colladaXml->FirstChildElement("asset");
        if (assetXml)
        {
          TiXmlElement *unitXml = assetXml->FirstChildElement("unit");
          if (unitXml && unitXml->Attribute("meter")){
                this->meter = math::parseFloat(unitXml->Attribute("meter"));
                std::cout << "meter: " << this->meter << std::endl;
          }
        }
        mesh = new Malla();
        mesh->setPath(this->filename);

        loadScene(mesh);

        mesh->Scale(this->meter*scale);

        if(to2D){
            mesh->Scale(scalemap);

            worldTo2D();

            mesh->Scale(1/scalemap);
        }
    }

    int ite = 0;

    void ColladaParser::draw()
    {

        if(ite==0){
            ite++;
            std::vector<std::string> listaNombres;
            std::vector<unsigned int> listaTexturas;
            for(int i = 0; i < this->mesh->getSubMeshCount();i++){
                SubMalla* submalla = this->mesh->getSubMesh(i);

                int indiceMaterial = submalla->getMaterialIndex();

                if(indiceMaterial==-1){
                    continue;
                }

                Material* material = this->mesh->getMaterial(indiceMaterial);

                std::string name = material->texImage;
                bool encontrado =false;
                int indiceEncontrado = 0;
                for(int j = 0; j < listaNombres.size(); j++){
                    if( listaNombres[j].compare(name)==0 ){
                        encontrado = true;
                        indiceEncontrado =j;
                        break;
                    }
                }

                if(!encontrado){
                    listaNombres.push_back(name);

                    unsigned int m_nID;
                    glGenTextures(1, &m_nID);
                    glBindTexture(GL_TEXTURE_2D, m_nID);

                    cv::Mat image = material->getImage();

                    gluBuild2DMipmaps( GL_TEXTURE_2D, 3,
                                   image.cols,
                                   image.rows,
                                   GL_BGR, GL_UNSIGNED_BYTE,
                                   image.data );
                    listaTexturas.push_back(m_nID);
                    material->idTextura = m_nID;
                }else{
                    material->idTextura = listaTexturas[indiceEncontrado];
                }
            }
        }

        for(int i = 0; i < this->mesh->getSubMeshCount();i++){

            SubMalla* submalla = this->mesh->getSubMesh(i);

            int indiceMaterial = submalla->getMaterialIndex();
            Material* material = this->mesh->getMaterial(indiceMaterial);

            if(submalla->getTexCoordCount()>0){
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, material->idTextura);
            }


            Color c;
            if(indiceMaterial>0){
                c = material->getDiffuse();
            }else{
                c.r = 0.7;
                c.g = 0.7;
                c.b = 0.7;
                c.a = 1;
            }



            if(submalla->getTexCoordCount()==0){
                glColor3f(c.r, c.g, c.b);

            }

            glPointSize(5.0);
            glBegin(GL_POLYGON);
            for(int k = 0; k < submalla->getVertexCount(); k++){

                if(k%3==0)
                    glBegin(GL_POLYGON);

                unsigned int indice =submalla->getIndex(k);

                math::Vector3 v= submalla->getVertex(indice);
                if(submalla->getNormalCount()>0){
                    math::Vector3 normals= submalla->getNormal(indice);
                    glNormal3f(normals.vector(0), normals.vector(1), normals.vector(2));
                }

                if(submalla->getTexCoordCount()>0){
                    math::Vector2d text= submalla->getTexCoord(indice);
                    glTexCoord2f(text.vector(0), text.vector(1));

                }
                glVertex3f(v.vector(0), v.vector(1), v.vector(2));

                if(k%3==2)
                    glEnd();

            }

            glEnd();

            if(submalla->getTexCoordCount()>0){
                glDisable(GL_TEXTURE_2D);
            }

        }
    }


    /////////////////////////////////////////////////
    void ColladaParser::loadScene(Malla *_mesh)
    {
      TiXmlElement *sceneXml = this->colladaXml->FirstChildElement("scene");
      std::string sceneURL =
        sceneXml->FirstChildElement("instance_visual_scene")->Attribute("url");

      TiXmlElement *visSceneXml = this->getElementId("visual_scene", sceneURL);

      if (!visSceneXml)
      {
        std::cout << "Unable to find visual_scene id ='" << sceneURL << "'\n";
        return;
      }

      TiXmlElement *nodeXml = visSceneXml->FirstChildElement("node");
      while (nodeXml)
      {
        this->loadNode(nodeXml, _mesh, math::Matriz4x4::IDENTITY);
        nodeXml = nodeXml->NextSiblingElement("node");
      }
    }

    /////////////////////////////////////////////////
    TiXmlElement *ColladaParser::getElementId(const std::string &_name,
                                              const std::string &_id)
    {
      return this->getElementId(this->colladaXml, _name, _id);
    }

    /////////////////////////////////////////////////
    TiXmlElement *ColladaParser::getElementId(TiXmlElement *_parent,
        const std::string &_name,
        const std::string &_id)
    {
      std::string id = _id;
      if (id.length() > 0 && id[0] == '#')
        id.erase(0, 1);

      if ((id.empty() && _parent->Value() == _name) ||
          (_parent->Attribute("id") && _parent->Attribute("id") == id) ||
          (_parent->Attribute("sid") && _parent->Attribute("sid") == id))
      {
        return _parent;
      }

      TiXmlElement *elem = _parent->FirstChildElement();
      while (elem)
      {
        TiXmlElement *result = this->getElementId(elem, _name, _id);
        if (result)
        {
          return result;
        }

        elem = elem->NextSiblingElement();
      }

      return NULL;
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadNode(TiXmlElement *_elem, Malla *_mesh,
        const math::Matriz4x4 &_transform)
    {
      TiXmlElement *nodeXml;
      TiXmlElement *instGeomXml;

      math::Matriz4x4 transform = this->loadNodeTransform(_elem);
      transform = _transform * transform;

      if (_elem->Attribute("name"))
      {
        this->currentNodeName = _elem->Attribute("name");
      }

      nodeXml = _elem->FirstChildElement("node");
      while (nodeXml)
      {
        this->loadNode(nodeXml, _mesh, transform);
        nodeXml = nodeXml->NextSiblingElement("node");
      }

      if (_elem->FirstChildElement("instance_node"))
      {
        std::string nodeURLStr =
          _elem->FirstChildElement("instance_node")->Attribute("url");

        nodeXml = this->getElementId("node", nodeURLStr);
        if (!nodeXml)
        {
          std::cout << "Unable to find node[" << nodeURLStr << "]\n";
          return;
        }
        this->loadNode(nodeXml, _mesh, transform);
        return;
      }
      else
        nodeXml = _elem;

      instGeomXml = nodeXml->FirstChildElement("instance_geometry");
      while (instGeomXml)
      {
        std::string geomURL = instGeomXml->Attribute("url");
        TiXmlElement *geomXml = this->getElementId("geometry", geomURL);

        this->materialMap.clear();
        TiXmlElement *bindMatXml, *techniqueXml, *matXml;
        bindMatXml = instGeomXml->FirstChildElement("bind_material");
        while (bindMatXml)
        {
          if ((techniqueXml = bindMatXml->FirstChildElement("technique_common")))
          {
            matXml = techniqueXml->FirstChildElement("instance_material");
            while (matXml)
            {
              std::string symbol = matXml->Attribute("symbol");
              std::string target = matXml->Attribute("target");
              this->materialMap[symbol] = target;
              matXml = matXml->NextSiblingElement("instance_material");
            }
          }
          bindMatXml = bindMatXml->NextSiblingElement("bind_material");
        }

        this->loadGeometry(geomXml, transform, _mesh);
        instGeomXml = instGeomXml->NextSiblingElement("instance_geometry");
      }

      TiXmlElement *instContrXml =
        nodeXml->FirstChildElement("instance_controller");
      while (instContrXml)
      {
        std::string contrURL = instContrXml->Attribute("url");
        TiXmlElement *contrXml = this->getElementId("controller", contrURL);

        TiXmlElement *instSkelXml = instContrXml->FirstChildElement("skeleton");
        std::string rootURL = instSkelXml->GetText();
        TiXmlElement *rootNodeXml = this->getElementId("node", rootURL);

        this->materialMap.clear();
        TiXmlElement *bindMatXml, *techniqueXml, *matXml;
        bindMatXml = instContrXml->FirstChildElement("bind_material");
        while (bindMatXml)
        {
          if ((techniqueXml = bindMatXml->FirstChildElement("technique_common")))
          {
            matXml = techniqueXml->FirstChildElement("instance_material");
            while (matXml)
            {
              std::string symbol = matXml->Attribute("symbol");
              std::string target = matXml->Attribute("target");
              this->materialMap[symbol] = target;
              matXml = matXml->NextSiblingElement("instance_material");
            }
          }
          bindMatXml = bindMatXml->NextSiblingElement("bind_material");
        }

        this->loadController(contrXml, rootNodeXml, transform, _mesh);
        instContrXml = instContrXml->NextSiblingElement("instance_controller");
      }
    }

    /////////////////////////////////////////////////
    math::Matriz4x4 ColladaParser::loadNodeTransform(TiXmlElement *_elem)
    {
      math::Matriz4x4 transform(math::Matriz4x4::IDENTITY);

      if (_elem->FirstChildElement("matrix"))
      {
        std::string matrixStr = _elem->FirstChildElement("matrix")->GetText();
        std::istringstream iss(matrixStr);
        std::vector<double> values(16);
        for (unsigned int i = 0; i < 16; i++)
          iss >> values[i];

        transform.set(values[0], values[1], values[2], values[3],
                      values[4], values[5], values[6], values[7],
                      values[8], values[9], values[10], values[11],
                      values[12], values[13], values[14], values[15]);
      }
      else
      {

        if (_elem->FirstChildElement("scale"))
        {
            std::string scaleStr = _elem->FirstChildElement("scale")->GetText();
            math::Vector3 scale;
            scale = boost::lexical_cast<math::Vector3>(scaleStr);
            math::Matriz4x4 scaleMat;
            scaleMat.setScale(scale);
            transform = transform * scaleMat;
        }

        if (_elem->FirstChildElement("translate"))
        {
          std::string transStr = _elem->FirstChildElement("translate")->GetText();
          math::Vector3 translate;
          translate = boost::lexical_cast<math::Vector3>(transStr);
          // translate *= this->meter;
          transform.setTranslate(translate);
        }

        TiXmlElement *rotateXml = _elem->FirstChildElement("rotate");

        while (rotateXml)
        {
          math::Matriz3x3 mat;
          math::Vector3 axis;
          double angle;

          std::string rotateStr = rotateXml->GetText();
          std::istringstream iss(rotateStr);

          float x, y, z;

          iss >> x >> y >> z;

          axis.setX(x);
          axis.setY(y);
          axis.setZ(z);

          iss >> angle;
          mat.setFromAxis(axis.getX(), axis.getY(), axis.getZ(), angle*3.1416/180);
          transform = transform * mat;

          rotateXml = rotateXml->NextSiblingElement("rotate");
        }


      }

      return transform;
    }


    /////////////////////////////////////////////////
    void ColladaParser::loadVertices(const std::string &_id,
                    const math::Matriz4x4 &_transform,
                    std::vector<math::Vector3> &_verts,
                    std::vector<math::Vector3> &_norms)
    {
      TiXmlElement *verticesXml = this->getElementId(this->colladaXml,
                                                     "vertices", _id);

      if (!verticesXml)
      {
        std::cout << "Unable to find vertices[" << _id << "] in collada file\n";
        return;
      }

      TiXmlElement *inputXml = verticesXml->FirstChildElement("input");
      while (inputXml)
      {
        std::string semantic = inputXml->Attribute("semantic");
        std::string sourceStr = inputXml->Attribute("source");
        if (semantic == "NORMAL")
        {
          this->loadNormals(sourceStr, _transform, _norms);
        }
        else if (semantic == "POSITION")
        {
          this->loadPositions(sourceStr, _transform, _verts);
        }

        inputXml = inputXml->NextSiblingElement("input");
      }
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadPositions(const std::string &_id,
                        const math::Matriz4x4 &_transform,
                        std::vector<math::Vector3> &_values)
    {
      TiXmlElement *sourceXml = this->getElementId("source", _id);
      TiXmlElement *floatArrayXml = sourceXml->FirstChildElement("float_array");
      if (!floatArrayXml)
      {
        std::cout  << "Vertex source missing float_array element\n";
        return;
      }
      std::string valueStr = floatArrayXml->GetText();

      std::vector<std::string> strs;
      std::vector<std::string>::iterator iter, end;
      boost::split(strs, valueStr, boost::is_any_of("   "));

      end = strs.end();
      for (iter = strs.begin(); iter != end; iter += 3)
      {
        math::Vector3 vec(math::parseFloat(*iter), math::parseFloat(*(iter+1)),
            math::parseFloat(*(iter+2)));
        vec = _transform * vec;
        _values.push_back(vec);
      }
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadNormals(const std::string &_id,
                                    const math::Matriz4x4 &_transform,
                                    std::vector<math::Vector3> &_values)
    {
      TiXmlElement *normalsXml = this->getElementId("source", _id);
      if (!normalsXml)
      {
        std::cout  << "Unable to find normals[" << _id << "] in collada file\n";
        return;
      }

      TiXmlElement *floatArrayXml = normalsXml->FirstChildElement("float_array");
      if (!floatArrayXml)
      {
        std::cout  << "Normal source missing float_array element\n";
        return;
      }

      std::string valueStr = floatArrayXml->GetText();
      std::istringstream iss(valueStr);
      do
      {
        math::Vector3 vec;
        float x, y, z;
        iss >> x >> y >> z;
        vec.setX(x);
        vec.setY(y);
        vec.setZ(z);

        if (iss)
        {
          vec = _transform * vec;
          vec.normalize();
          _values.push_back(vec);
        }
      } while (iss);
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadGeometry(TiXmlElement *_xml,
                                     const math::Matriz4x4 &_transform, Malla *_mesh)
    {
      TiXmlElement *meshXml = _xml->FirstChildElement("mesh");
      TiXmlElement *childXml;

      if (!meshXml)
        return;

      childXml = meshXml->FirstChildElement("triangles");
      while (childXml)
      {
        this->loadTriangles(childXml, _transform, _mesh);
        childXml = childXml->NextSiblingElement("triangles");
      }

      childXml = meshXml->FirstChildElement("polylist");
      while (childXml)
      {
        this->loadPolylist(childXml, _transform, _mesh);
        childXml = childXml->NextSiblingElement("polylist");
      }

      childXml = meshXml->FirstChildElement("lines");
      while (childXml)
      {
        this->loadLines(childXml, _transform, _mesh);
        childXml = childXml->NextSiblingElement("lines");
      }
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadLines(TiXmlElement *_xml,
        const math::Matriz4x4 &_transform,
        Malla *_mesh)
    {
      SubMalla *subMalla = new SubMalla();
      subMalla->setName(this->currentNodeName);
      subMalla->setPrimitiveType(SubMalla::LINES);

      TiXmlElement *inputXml = _xml->FirstChildElement("input");
      // std::string semantic = inputXml->Attribute("semantic");
      std::string source = inputXml->Attribute("source");

      std::vector<math::Vector3> verts;
      std::vector<math::Vector3> norms;
      this->loadVertices(source, _transform, verts, norms);

      TiXmlElement *pXml = _xml->FirstChildElement("p");
      std::string pStr = pXml->GetText();
      std::istringstream iss(pStr);

      do
      {
        int a, b;
        iss >> a >> b;

        if (!iss)
          break;
        subMalla->addVertex(verts[a]);
        subMalla->addIndex(subMalla->getVertexCount() - 1);
        subMalla->addVertex(verts[b]);
        subMalla->addIndex(subMalla->getVertexCount() - 1);
      } while (iss);

      _mesh->addSubMalla(subMalla);
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadPolylist(TiXmlElement *_polylistXml,
        const math::Matriz4x4 &_transform,
        Malla *_mesh)
    {
      // This function parses polylist types in collada into
      // a set of triangle meshes.  The assumption is that
      // each polylist polygon is convex, and we do decomposion
      // by anchoring each triangle about vertex 0 or each polygon
      SubMalla *subMalla = new SubMalla;
      subMalla->setName(this->currentNodeName);
      bool combinedVertNorms = false;

      subMalla->setPrimitiveType(SubMalla::TRIANGLES);

      if (_polylistXml->Attribute("material"))
      {
        std::map<std::string, std::string>::iterator iter;
        std::string matStr = _polylistXml->Attribute("material");

        iter = this->materialMap.find(matStr);
        if (iter != this->materialMap.end())
          matStr = iter->second;

        int matIndex = _mesh->addMaterial(this->loadMaterial(matStr));
        if (matIndex < 0)
          std::cout << "Unable to add material[" << matStr << "]\n";
        else
          subMalla->setMaterialIndex(matIndex);
      }

      TiXmlElement *polylistInputXml = _polylistXml->FirstChildElement("input");

      std::vector<math::Vector3> verts;
      std::vector<math::Vector3> norms;
      std::vector<math::Vector2d> texcoords;

      math::Matriz4x4 bindShapeMat(math::Matriz4x4::IDENTITY);
      //if (_mesh->HasSkeleton())
      //  bindShapeMat = _mesh->GetSkeleton()->GetBindShapeTransform();

      // read input elements
      std::map<std::string, int> inputs;
      while (polylistInputXml)
      {
        std::string semantic = polylistInputXml->Attribute("semantic");
        std::string source = polylistInputXml->Attribute("source");
        std::string offset = polylistInputXml->Attribute("offset");
        if (semantic == "VERTEX")
        {
          unsigned int count = norms.size();
          this->loadVertices(source, _transform, verts, norms);
          if (norms.size() > count)
            combinedVertNorms = true;
        }
        else if (semantic == "NORMAL")
        {
          this->loadNormals(source, _transform, norms);
          combinedVertNorms = false;
        }
        else if (semantic == "TEXCOORD")
          this->loadTexCoords(source, texcoords);

        inputs[semantic] = math::parseInt(offset);

        polylistInputXml = polylistInputXml->NextSiblingElement("input");
      }

      // read vcount
      // break poly into triangles
      // if vcount >= 4, anchor around 0 (note this is bad for concave elements)
      //   e.g. if vcount = 4, break into triangle 1: [0,1,2], triangle 2: [0,2,3]
      std::vector<std::string> vcountStrs;
      TiXmlElement *vcountXml = _polylistXml->FirstChildElement("vcount");
      std::string vcountStr = vcountXml->GetText();
      boost::split(vcountStrs, vcountStr, boost::is_any_of("   "));
      std::vector<int> vcounts;
      for (unsigned int j = 0; j < vcountStrs.size(); ++j)
        vcounts.push_back(math::parseInt(vcountStrs[j]));

      // read p
      TiXmlElement *pXml = _polylistXml->FirstChildElement("p");
      std::string pStr = pXml->GetText();

      std::vector<math::Vector3> vertNorms(verts.size());
      std::vector<int> vertNormsCounts(verts.size());
      std::fill(vertNormsCounts.begin(), vertNormsCounts.end(), 0);

      int *values = new int[inputs.size()];
      std::map<std::string, int>::iterator end = inputs.end();
      std::map<std::string, int>::iterator iter;
      math::Vector2d vec;

      std::vector<std::string> strs;
      boost::split(strs, pStr, boost::is_any_of("   "));
      std::vector<std::string>::iterator strs_iter = strs.begin();
      for (unsigned int l = 0; l < vcounts.size(); ++l)
      {
        // put us at the beginning of the polygon list
        if (l > 0) strs_iter += inputs.size()*vcounts[l-1];

        for (unsigned int k = 2; k < (unsigned int)vcounts[l]; ++k)
        {
          // if vcounts[l] = 5, then read 0,1,2, then 0,2,3, 0,3,4,...
          // here k = the last number in the series
          // j is the triangle loop
          for (unsigned int j = 0; j < 3; ++j)
          {
            // break polygon into triangles
            unsigned int triangle_index;

            if (j == 0)
              triangle_index = 0;
            if (j == 1)
              triangle_index = (k-1)*inputs.size();
            if (j == 2)
              triangle_index = (k)*inputs.size();

            for (unsigned int i = 0; i < inputs.size(); i++)
            {
              values[i] = math::parseInt(strs_iter[triangle_index+i]);
    //          std::cout << "debug parsing "
    //                << " poly-i[" << l
    //                << "] tri-end-index[" << k
    //                << "] tri-vertex-i[" << j
    //                << "] triangle[" << triangle_index
    //                << "] input[" << i
    //                << "] value[" << values[i]
    //                << "]\n";
            }


            for (iter = inputs.begin(); iter != end; ++iter)
            {
              if (iter->first == "VERTEX")
              {
                subMalla->addVertex(bindShapeMat * verts[values[iter->second]]);
                subMalla->addIndex(subMalla->getVertexCount()-1);
                if (combinedVertNorms)
                  subMalla->addNormal(norms[values[iter->second]]);
    //            if (_mesh->HasSkeleton())
    //            {
    //              Skeleton *skel = _mesh->GetSkeleton();
    //              for (unsigned int i = 0;
    //                  i < skel->GetNumVertNodeWeights(values[iter->second]); i++)
    //              {
    //                std::pair<std::string, double> node_weight =
    //                              skel->GetVertNodeWeight(values[iter->second], i);
    //                SkeletonNode *node =
    //                  _mesh->GetSkeleton()->GetNodeByName(node_weight.first);
    //                SubMalla->AddNodeAssignment(SubMalla->GetVertexCount()-1,
    //                            node->GetHandle(), node_weight.second);
    //              }
    //            }
              }
              else if (iter->first == "NORMAL")
              {
                subMalla->addNormal(norms[values[iter->second]]);
              }
              else if (iter->first == "TEXCOORD")
              {
                  subMalla->addTexCoord(texcoords[values[iter->second]].getX(),
                                        texcoords[values[iter->second]].getY());
              }
              // else
              // gzerr << "Unhandled semantic[" << iter->first << "]\n";
            }
          }
        }
      }
      delete [] values;

      _mesh->addSubMalla(subMalla);
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadTexCoords(const std::string &_id,
                                      std::vector<math::Vector2d> &_values)
    {
      int stride = 0;
      int texCount = 0;
      int totCount = 0;

      // Get the source element for the texture coordinates.
      TiXmlElement *xml = this->getElementId("source", _id);
      if (!xml)
      {
        std::cout << "Unable to find tex coords[" << _id << "] in collada file\n";
        return;
      }

      // Get the array of float values. These are the raw values for the texture
      // coordinates.
      TiXmlElement *floatArrayXml = xml->FirstChildElement("float_array");
      if (!floatArrayXml)
      {
        std::cout << "Normal source missing float_array element\n";
        return;
      }

      // The technique_common holds an <accessor> element that indicates how to
      // parse the float array.
      xml = xml->FirstChildElement("technique_common");
      if (!xml)
      {
        std::cout << "Unable to find technique_common element for texture "
              << "coordinates with id[" << _id << "]\n";
        return;
      }

      // Get the accessor XML element.
      xml = xml->FirstChildElement("accessor");
      if (!xml)
      {
        std::cout << "Unable to find <accessor> as a child of <technique_common> "
              << "for texture coordinates with id[" << _id << "]\n";
        return;
      }

      // Read in the total number of texture coordinate values
      if (floatArrayXml->Attribute("count"))
        totCount = boost::lexical_cast<int>(floatArrayXml->Attribute("count"));
      else
      {
        std::cout << "<float_array> has no count attribute in texture coordinate "
              << "element with id[" << _id << "]\n";
        return;
      }

      // Read in the stride for the texture coordinate values. The stride
      // indicates the number of values in the float array the comprise
      // a complete texture coordinate.
      if (xml->Attribute("stride"))
        stride = boost::lexical_cast<int>(xml->Attribute("stride"));
      else
      {
        std::cout << "<accessor> has no stride attribute in texture coordinate element "
              << "with id[" << _id << "]\n";
        return;
      }

      // Read in the count of texture coordinates.
      if (xml->Attribute("count"))
        texCount = boost::lexical_cast<int>(xml->Attribute("count"));
      else
      {
        std::cout << "<accessor> has no count attribute in texture coordinate element "
              << "with id[" << _id << "]\n";
        return;
      }

      // \TODO This is a good a GZ_ASSERT
      // The total number of texture values should equal the stride multiplied
      // by the number of texture coordinates.
      if (texCount * stride != totCount)
      {
        std::cout << "Error reading texture coordinates. Coordinate counts in element "
                 "with id[" << _id << "] do not add up correctly\n";
        return;
      }

      // Read the raw texture values, and split them on spaces.
      std::string valueStr = floatArrayXml->GetText();
      std::vector<std::string> values;
      boost::split(values, valueStr, boost::is_any_of(" "));

      // Read in all the texture coordinates.
      for (int i = 0; i < totCount; i += stride)
      {
        // We only handle 2D texture coordinates right now.
        _values.push_back(math::Vector2d(boost::lexical_cast<double>(values[i]),
              1.0 - boost::lexical_cast<double>(values[i+1])));
      }
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadTriangles(TiXmlElement *_trianglesXml,
                                      const math::Matriz4x4 &_transform,
                                      Malla *_mesh)
    {
      SubMalla *subMalla = new SubMalla;
      subMalla->setName(this->currentNodeName);
      bool combinedVertNorms = false;

      subMalla->setPrimitiveType(SubMalla::TRIANGLES);

      if (_trianglesXml->Attribute("material"))
      {
        std::map<std::string, std::string>::iterator iter;
        std::string matStr = _trianglesXml->Attribute("material");

        iter = this->materialMap.find(matStr);
        if (iter != this->materialMap.end())
          matStr = iter->second;

        int matIndex = _mesh->addMaterial(this->loadMaterial(matStr));
        if (matIndex < 0)
          std::cout  << "Unable to add material[" << matStr << "]\n";
        else
          subMalla->setMaterialIndex(matIndex);
      }

      TiXmlElement *trianglesInputXml = _trianglesXml->FirstChildElement("input");

      std::vector<math::Vector3> verts;
      std::vector<math::Vector3> norms;
      std::vector<math::Vector2d> texcoords;

      // A list of all the input values.
      std::list<std::pair<std::string, int> > inputs;
      while (trianglesInputXml)
      {
        std::string semantic = trianglesInputXml->Attribute("semantic");
        std::string source = trianglesInputXml->Attribute("source");
        std::string offset = trianglesInputXml->Attribute("offset");
        if (semantic == "VERTEX")
        {
          unsigned int count = norms.size();
          this->loadVertices(source, _transform, verts, norms);
          if (norms.size() > count)
            combinedVertNorms = true;
        }
        else if (semantic == "NORMAL")
        {
          this->loadNormals(source, _transform, norms);
          combinedVertNorms = false;
        }
        else if (semantic == "TEXCOORD")
          this->loadTexCoords(source, texcoords);

        inputs.push_back(std::make_pair(semantic, math::parseInt(offset)));

        trianglesInputXml = trianglesInputXml->NextSiblingElement("input");
      }

      TiXmlElement *pXml = _trianglesXml->FirstChildElement("p");
      if (!pXml || !pXml->GetText())
      {
        std::cout << "Collada file[" << this->filename
              << "] is invalid. Loading what we can...\n";
        return;
      }
      std::string pStr = pXml->GetText();

      std::vector<math::Vector3> vertNorms(verts.size());
      std::vector<int> vertNormsCounts(verts.size());
      std::fill(vertNormsCounts.begin(), vertNormsCounts.end(), 0);

      int *values = new int[inputs.size()];
      std::list<std::pair<std::string, int> >::iterator end = inputs.end();
      std::list<std::pair<std::string, int> >::iterator iter;
      math::Vector2d vec;

      std::vector<std::string> strs;
      boost::split(strs, pStr, boost::is_any_of("   "));

      for (unsigned int j = 0; j < strs.size(); j += inputs.size())
      {
        for (unsigned int i = 0; i < inputs.size(); i++)
          values[i] = math::parseInt(strs[j+i]);

        bool already = false;
        for (iter = inputs.begin(); iter != end; ++iter)
        {
          if ((*iter).first == "VERTEX")
          {
            subMalla->addVertex(verts[values[(*iter).second]]);
            subMalla->addIndex(subMalla->getVertexCount()-1);
            if (combinedVertNorms)
              subMalla->addNormal(norms[values[(*iter).second]]);
    //        if (_mesh->HasSkeleton())
    //        {
    //          Skeleton *skel = _mesh->GetSkeleton();
    //          for (unsigned int i = 0;
    //                  i < skel->GetNumVertNodeWeights(values[(*iter).second]); i++)
    //          {
    //            std::pair<std::string, double> node_weight =
    //              skel->GetVertNodeWeight(values[(*iter).second], i);
    //            SkeletonNode *node =
    //                _mesh->GetSkeleton()->GetNodeByName(node_weight.first);
    //            SubMalla->AddNodeAssignment(SubMalla->GetVertexCount()-1,
    //                            node->GetHandle(), node_weight.second);
    //          }
    //        }
          }
          else if ((*iter).first == "NORMAL")
          {
            subMalla->addNormal(norms[values[(*iter).second]]);
          }
          else if ((*iter).first == "TEXCOORD" && !already)
          {
            already = true;
            subMalla->addTexCoord(texcoords[values[(*iter).second]].getX(),
                                  texcoords[values[(*iter).second]].getY());
          }
          // else
          // gzerr << "Unhandled semantic[" << (*iter).first << "]\n";
        }
      }
      delete [] values;

      _mesh->addSubMalla(subMalla);
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadController(TiXmlElement *_contrXml,
          TiXmlElement *_skelXml, const math::Matriz4x4 _transform, Malla *_mesh)
    {
    }
    //{
    //  Skeleton *skeleton = new Skeleton(this->LoadSkeletonNodes(_skelXml, NULL));
    //  _mesh->SetSkeleton(skeleton);

    //  TiXmlElement *rootXml = _contrXml->GetDocument()->RootElement();

    //  if (rootXml->FirstChildElement("library_animations"))
    //    this->LoadAnimations(rootXml->FirstChildElement("library_animations"),
    //        skeleton);

    //  TiXmlElement *skinXml = _contrXml->FirstChildElement("skin");
    //  std::string geomURL = skinXml->Attribute("source");

    //  math::Matrix4 bindTrans;
    //  std::string matrixStr =
    //        skinXml->FirstChildElement("bind_shape_matrix")->GetText();
    //  std::istringstream iss(matrixStr);
    //  std::vector<double> values(16);
    //  for (unsigned int i = 0; i < 16; i++)
    //    iss >> values[i];
    //  bindTrans.Set(values[0], values[1], values[2], values[3],
    //                values[4], values[5], values[6], values[7],
    //                values[8], values[9], values[10], values[11],
    //                values[12], values[13], values[14], values[15]);

    //  skeleton->SetBindShapeTransform(bindTrans);

    //  TiXmlElement *jointsXml = skinXml->FirstChildElement("joints");
    //  std::string jointsURL, invBindMatURL;
    //  TiXmlElement *inputXml = jointsXml->FirstChildElement("input");
    //  while (inputXml)
    //  {
    //    std::string semantic = inputXml->Attribute("semantic");
    //    std::string source = inputXml->Attribute("source");
    //    if (semantic == "JOINT")
    //      jointsURL = source;
    //    else
    //    {
    //      if (semantic == "INV_BIND_MATRIX")
    //        invBindMatURL = source;
    //    }
    //    inputXml = inputXml->NextSiblingElement("input");
    //  }

    //  jointsXml = this->GetElementId("source", jointsURL);

    //  if (!jointsXml)
    //  {
    //    gzerr << "Could not find node[" << jointsURL << "]\n";
    //    gzthrow("Faild to parse skinning information in Collada file.");
    //  }

    //  std::string jointsStr = jointsXml->FirstChildElement("Name_array")->GetText();

    //  std::vector<std::string> joints;
    //  boost::split(joints, jointsStr, boost::is_any_of("   "));

    //  TiXmlElement *invBMXml = this->GetElementId("source", invBindMatURL);

    //  if (!invBMXml)
    //  {
    //    gzerr << "Could not find node[" << invBindMatURL << "]\n";
    //    gzthrow("Faild to parse skinning information in Collada file.");
    //  }

    //  std::string posesStr = invBMXml->FirstChildElement("float_array")->GetText();

    //  std::vector<std::string> strs;
    //  boost::split(strs, posesStr, boost::is_any_of("   "));

    //  for (unsigned int i = 0; i < joints.size(); i++)
    //  {
    //    unsigned int id = i * 16;
    //    math::Matrix4 mat;
    //    mat.Set(math::parseFloat(strs[id +  0]), math::parseFloat(strs[id +  1]),
    //            math::parseFloat(strs[id +  2]), math::parseFloat(strs[id +  3]),
    //            math::parseFloat(strs[id +  4]), math::parseFloat(strs[id +  5]),
    //            math::parseFloat(strs[id +  6]), math::parseFloat(strs[id +  7]),
    //            math::parseFloat(strs[id +  8]), math::parseFloat(strs[id +  9]),
    //            math::parseFloat(strs[id + 10]), math::parseFloat(strs[id + 11]),
    //            math::parseFloat(strs[id + 12]), math::parseFloat(strs[id + 13]),
    //            math::parseFloat(strs[id + 14]), math::parseFloat(strs[id + 15]));

    //    skeleton->GetNodeByName(joints[i])->SetInverseBindTransform(mat);
    //  }

    //  TiXmlElement *vertWeightsXml = skinXml->FirstChildElement("vertex_weights");

    //  inputXml = vertWeightsXml->FirstChildElement("input");
    //  unsigned int jOffset = 0;
    //  unsigned int wOffset = 0;
    //  std::string weightsURL;
    //  while (inputXml)
    //  {
    //    std::string semantic = inputXml->Attribute("semantic");
    //    std::string source = inputXml->Attribute("source");
    //    int offset;
    //    inputXml->Attribute("offset", &offset);

    //    if (semantic == "JOINT")
    //      jOffset = offset;
    //    else
    //      if (semantic == "WEIGHT")
    //      {
    //        weightsURL = source;
    //        wOffset = offset;
    //      }
    //    inputXml = inputXml->NextSiblingElement("input");
    //  }

    //  TiXmlElement *weightsXml = this->GetElementId("source", weightsURL);

    //  std::string wString = weightsXml->FirstChildElement("float_array")->GetText();
    //  std::vector<std::string> wStrs;
    //  boost::split(wStrs, wString, boost::is_any_of("   "));

    //  std::vector<float> weights;
    //  for (unsigned int i = 0; i < wStrs.size(); i++)
    //    weights.push_back(math::parseFloat(wStrs[i]));

    //  std::string cString = vertWeightsXml->FirstChildElement("vcount")->GetText();
    //  std::string vString = vertWeightsXml->FirstChildElement("v")->GetText();
    //  std::vector<std::string> vCountStrs;
    //  std::vector<std::string> vStrs;

    //  boost::split(vCountStrs, cString, boost::is_any_of("   "));
    //  boost::split(vStrs, vString, boost::is_any_of("   "));

    //  std::vector<unsigned int> vCount;
    //  std::vector<unsigned int> v;

    //  for (unsigned int i = 0; i < vCountStrs.size(); i++)
    //    vCount.push_back(math::parseInt(vCountStrs[i]));

    //  for (unsigned int i = 0; i < vStrs.size(); i++)
    //    v.push_back(math::parseInt(vStrs[i]));

    //  skeleton->SetNumVertAttached(vCount.size());

    //  unsigned int vIndex = 0;
    //  for (unsigned int i = 0; i < vCount.size(); i++)
    //  {
    //    for (unsigned int j = 0; j < vCount[i]; j++)
    //    {
    //      skeleton->AddVertNodeWeight(i, joints[v[vIndex + jOffset]],
    //                                    weights[v[vIndex + wOffset]]);
    //      vIndex += (jOffset + wOffset + 1);
    //    }
    //  }

    //  TiXmlElement *geomXml = this->GetElementId("geometry", geomURL);
    //  this->LoadGeometry(geomXml, _transform, _mesh);
    //}

    /////////////////////////////////////////////////
    void ColladaParser::loadColorOrTexture(TiXmlElement *_elem,
        const std::string &_type, Material *_mat)
    {
      if (!_elem || !_elem->FirstChildElement(_type))
        return;

      TiXmlElement *typeElem = _elem->FirstChildElement(_type);

      if (typeElem->FirstChildElement("color"))
      {
        std::string colorStr = typeElem->FirstChildElement("color")->GetText();
        Color color = boost::lexical_cast<Color>(colorStr);
        if (_type == "diffuse")
          _mat->setDiffuse(color);
        else if (_type == "ambient")
          _mat->setAmbient(color);
        else if (_type == "emission")
          _mat->setEmissive(color);
      }
      else if (typeElem->FirstChildElement("texture"))
      {
        TiXmlElement *imageXml = NULL;
        std::string textureName =
          typeElem->FirstChildElement("texture")->Attribute("texture");
        TiXmlElement *textureXml = this->getElementId("newparam", textureName);
        if (textureXml)
        {
          if (std::string(textureXml->Value()) == "image")
          {
            imageXml = textureXml;
          }
          else
          {
            TiXmlElement *sampler = textureXml->FirstChildElement("sampler2D");
            if (sampler)
            {
              std::string sourceName =
                sampler->FirstChildElement("source")->GetText();
              TiXmlElement *sourceXml = this->getElementId("newparam", sourceName);
              if (sourceXml)
              {
                TiXmlElement *surfaceXml = sourceXml->FirstChildElement("surface");
                if (surfaceXml && surfaceXml->FirstChildElement("init_from"))
                {
                  imageXml = this->getElementId("image",
                      surfaceXml->FirstChildElement("init_from")->GetText());
                }
              }
            }
          }
        }
        else
        {
          imageXml = this->getElementId("image", textureName);
        }

        if (imageXml && imageXml->FirstChildElement("init_from"))
        {
          std::string imgFile =
            imageXml->FirstChildElement("init_from")->GetText();

          _mat->setTextureImage(imgFile, this->path);
        }
      }
    }

    float ColladaParser::loadFloat(TiXmlElement *_elem)
    {
      float value = 0;

      if (_elem->FirstChildElement("float"))
      {
        value = math::parseFloat(_elem->FirstChildElement("float")->GetText());
      }

      return value;
    }

    /////////////////////////////////////////////////
    void ColladaParser::loadTransparent(TiXmlElement *_elem, Material *_mat)
    {
      const char *opaqueCStr = _elem->Attribute("opaque");
      if (!opaqueCStr)
      {
        // gzerr << "No Opaque set\n";
        return;
      }

      // TODO: Handle transparent textures
      if (_elem->FirstChildElement("color"))
      {
        const char *colorCStr = _elem->FirstChildElement("color")->GetText();
        if (!colorCStr)
        {
          std::cout  << "No color string\n";
          return;
        }

        std::string opaqueStr = opaqueCStr;
        std::string colorStr = colorCStr;
        Color color = boost::lexical_cast<Color>(colorStr);

        double srcFactor = 0;
        double dstFactor = 0;

        if (opaqueStr == "RGB_ZERO")
        {
          srcFactor = color.r * _mat->getTransparency();
          dstFactor = 1.0 - color.r * _mat->getTransparency();
        }
        else if (opaqueStr == "A_ONE")
        {
          srcFactor = 1.0 - color.a * _mat->getTransparency();
          dstFactor = color.a * _mat->getTransparency();
        }

        _mat->setBlendFactors(srcFactor, dstFactor);
      }
    }


    /////////////////////////////////////////////////
    Material *ColladaParser::loadMaterial(const std::string &_name)
    {
      TiXmlElement *matXml = this->getElementId("material", _name);
      if (!matXml || !matXml->FirstChildElement("instance_effect"))
        return NULL;

      Material *mat = new Material();
      std::string effectName =
        matXml->FirstChildElement("instance_effect")->Attribute("url");
      TiXmlElement *effectXml = this->getElementId("effect", effectName);

      TiXmlElement *commonXml = effectXml->FirstChildElement("profile_COMMON");
      if (commonXml)
      {
        TiXmlElement *techniqueXml = commonXml->FirstChildElement("technique");
        TiXmlElement *lambertXml = techniqueXml->FirstChildElement("lambert");

        TiXmlElement *phongXml = techniqueXml->FirstChildElement("phong");
        TiXmlElement *blinnXml = techniqueXml->FirstChildElement("blinn");
        if (lambertXml)
        {
          this->loadColorOrTexture(lambertXml, "ambient", mat);
          this->loadColorOrTexture(lambertXml, "emission", mat);
          this->loadColorOrTexture(lambertXml, "diffuse", mat);
          if (lambertXml->FirstChildElement("transparency"))
          {
            mat->setTransparency(
                this->loadFloat(lambertXml->FirstChildElement("transparency")));
          }

          if (lambertXml->FirstChildElement("transparent"))
          {
            TiXmlElement *transXml = lambertXml->FirstChildElement("transparent");
            this->loadTransparent(transXml, mat);
          }
        }
        else if (phongXml)
        {
          this->loadColorOrTexture(phongXml, "ambient", mat);
          this->loadColorOrTexture(phongXml, "emission", mat);
          this->loadColorOrTexture(phongXml, "specular", mat);
          this->loadColorOrTexture(phongXml, "diffuse", mat);
          if (phongXml->FirstChildElement("shininess"))
            mat->setShininess(
                this->loadFloat(phongXml->FirstChildElement("shininess")));

          if (phongXml->FirstChildElement("transparency"))
            mat->setTransparency(
                this->loadFloat(phongXml->FirstChildElement("transparency")));
          if (phongXml->FirstChildElement("transparent"))
          {
            TiXmlElement *transXml = phongXml->FirstChildElement("transparent");
            this->loadTransparent(transXml, mat);
          }
        }
        else if (blinnXml)
        {
          this->loadColorOrTexture(blinnXml, "ambient", mat);
          this->loadColorOrTexture(blinnXml, "emission", mat);
          this->loadColorOrTexture(blinnXml, "specular", mat);
          this->loadColorOrTexture(blinnXml, "diffuse", mat);
          if (blinnXml->FirstChildElement("shininess"))
            mat->setShininess(
                this->loadFloat(blinnXml->FirstChildElement("shininess")));

          if (blinnXml->FirstChildElement("transparency"))
            mat->setTransparency(
                this->loadFloat(blinnXml->FirstChildElement("transparency")));
          if (blinnXml->FirstChildElement("transparent"))
          {
            TiXmlElement *transXml = blinnXml->FirstChildElement("transparent");
            this->loadTransparent(transXml, mat);
          }
        }
      }

      TiXmlElement *glslXml = effectXml->FirstChildElement("profile_GLSL");
      if (glslXml)
        std::cout  << "profile_GLSL unsupported\n";

      TiXmlElement *cgXml = effectXml->FirstChildElement("profile_CG");
      if (cgXml)
        std::cout  << "profile_CG unsupported\n";
      return mat;
    }

    void ColladaParser::worldTo2D()
    {

        std::vector<Segmento> listaSegmentos;

        for(int i = 0; i < this->mesh->getSubMeshCount();i++){

            SubMalla* submalla = this->mesh->getSubMesh(i);

            Plano plano1(0, 0, 1, -10);
            Plano plano2(0, 0, 1, 400.0);
            Plano p_proyeccion(0, 0, 1, 0);

            math::Vector3 inter1;

            int triangulo = 0;
            int indice_triangulo = 0;

            for(int k = 0; k < submalla->getVertexCount()-1; k++){
                unsigned int indice =submalla->getIndex(k);

                triangulo++;
                if(triangulo == 3){
                    indice_triangulo++;
                    triangulo = 0;
                }

                math::Vector3 v1= submalla->getVertex(indice%3 + indice_triangulo*3);
                math::Vector3 v2= submalla->getVertex((indice+1)%3 + indice_triangulo*3);

                inter1 = plano1.InterConRecta(v1, v2);

                if(inter1.getX()!=0 && inter1.getY()!=0 && inter1.getZ()!=0 ){
                    math::Vector3 proyeccion1 = p_proyeccion.proyeccionOrtogonal(v1, p_proyeccion.getCoefA(), p_proyeccion.getCoefB(), p_proyeccion.getCoefC());
                    math::Vector3 proyeccion2 = p_proyeccion.proyeccionOrtogonal(v2, p_proyeccion.getCoefA(), p_proyeccion.getCoefB(), p_proyeccion.getCoefC());

                    double dist = proyeccion1.distance(proyeccion2);

                    if(dist > 1){
                        listaSegmentos.push_back(Segmento(proyeccion1,proyeccion2));
                    }
                }

                inter1 = plano2.InterConRecta(v1, v2);

                if(inter1.getX()!=0 && inter1.getY()!=0 && inter1.getZ()!=0 ){
                    math::Vector3 proyeccion1 = p_proyeccion.proyeccionOrtogonal(v1, p_proyeccion.getCoefA(), p_proyeccion.getCoefB(), p_proyeccion.getCoefC());
                    math::Vector3 proyeccion2 = p_proyeccion.proyeccionOrtogonal(v2, p_proyeccion.getCoefA(), p_proyeccion.getCoefB(), p_proyeccion.getCoefC());

                    double dist = proyeccion1.distance(proyeccion2);

                    if(dist > 1){
                        listaSegmentos.push_back(Segmento(proyeccion1,proyeccion2));
                    }
                }
            }

//            int verticesDentroSandwich = 0;
//            float distanciaEntrePlanos = plano2.distanciaAPunto(0, 0, 1);
//            std::vector<math::Vector3> vectorProyecciones;

//            for(int k = 0; k < submalla->getVertexCount()-1; k++){
//                unsigned int indice =submalla->getIndex(k);

//                math::Vector3 v1= submalla->getVertex(indice);

//                float distanciaEntrePlanos1_1 = plano1.distanciaAPunto(v1);

//                float distanciaEntrePlanos1_2 = plano2.distanciaAPunto(v1);

//                if(fabs(distanciaEntrePlanos1_1+distanciaEntrePlanos1_2)<=fabs(distanciaEntrePlanos)){
//                    verticesDentroSandwich++;
//                    vectorProyecciones.push_back(p_proyeccion.proyeccionOrtogonal(v1,
//                                                                                  p_proyeccion.getCoefA(),
//                                                                                  p_proyeccion.getCoefB(),
//                                                                                  p_proyeccion.getCoefC()));
//                }
//            }

//            if(submalla->getVertexCount()==verticesDentroSandwich){
//                for(int k = 0; k < submalla->getVertexCount()-1; k++){
//                    unsigned int indice =submalla->getIndex(k);

//                    math::Vector3 v1= submalla->getVertex(indice);
//                    math::Vector3 v2= submalla->getVertex(indice);

//                    float dist = v1.distance(v2);

//                    if(dist > 2){
//                        listaSegmentos.push_back(Segmento( v1, v2));
//                    }
//                }
//            }
        }


        math::Vector3 max = mesh->getMax();
        math::Vector3 min = mesh->getMin();
        image.create((max.getY() - min.getY()),
                     (max.getX() - min.getY()),
                     CV_8UC3);

        image = cv::Scalar(255, 255, 255);

        FILE* fp;
        fp = fopen("lineas.txt", "w");

        for(int i = 0; i < listaSegmentos.size(); i++){
            cv::line(image,
                     cv::Point2f((listaSegmentos[i].x1 - min.getX()), (listaSegmentos[i].y1 - min.getY())),
                     cv::Point2f((listaSegmentos[i].x2 - min.getX()), (listaSegmentos[i].y2 - min.getY())),
                     cv::Scalar(0, 0, 0),
                     3);
            fprintf(fp, "%.2f %.2f %.2f %.2f\n", listaSegmentos[i].x1, listaSegmentos[i].y1,
                            listaSegmentos[i].x2, listaSegmentos[i].y2);
        }
        cv::imwrite("mapa.jpg", image);

        fclose(fp);

    }

    cv::Mat ColladaParser::getWorld2D()
    {
        return this->image.clone();
    }

}


