#include <jde+/schemaimplementation.h>
#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
using ost::MutexLock;
#endif
#include <tinyxml.h>
#include <iostream>

Schemaimplementation::Schemaimplementation(const schemaimplementation_info_t* implementation_info)
  :implementation_info(implementation_info) {}

/*Autoload mechanisms*/

std::map<std::string, int> Schemaimplementation::info_by_classname;
std::map<std::string, int> Schemaimplementation::info_by_inteface;
std::vector<Schemaimplementation::schemaimplementation_info_t> 
Schemaimplementation::class_infos;
static Mutex class_info_mutex;/*Global mutex used to access static
				data. Defined hear to avoid header
				dependencies in schemaimplementation.h*/

Schemaimplementation*
Schemaimplementation::instance(const std::string& class_name) throw(){
  const Schemaimplementation::schemaimplementation_info_t* cinfo = 
    Schemaimplementation::class_info(class_name);

  if (cinfo != 0)
    return cinfo->constructor();
  return 0;
}

const Schemaimplementation::schemaimplementation_info_t*
Schemaimplementation::class_info(const std::string& class_name) throw(){
  MutexLock l(class_info_mutex);

  return Schemaimplementation::class_info_i(class_name);
}

const Schemaimplementation::schemaimplementation_info_t*
Schemaimplementation::class_info_i(const std::string& class_name) throw(){
  std::map<std::string, int>::const_iterator f_it;

  f_it = Schemaimplementation::info_by_classname.find(class_name);
  if (f_it != Schemaimplementation::info_by_classname.end())
    return &(Schemaimplementation::class_infos[f_it->second]);
  return 0;
}

const Schemaimplementation::schemaimplementation_info_t*
Schemaimplementation::class_info_search(const std::string& interface_name,
					const std::vector<std::string>& branch_preferences) throw(){
  MutexLock l(class_info_mutex);

  return Schemaimplementation::class_info_search_i(interface_name,
						   branch_preferences);
}

const Schemaimplementation::schemaimplementation_info_t*
Schemaimplementation::class_info_search_i(const std::string& interface_name,
					  const std::vector<std::string>& branch_preferences) throw(){
  std::vector<std::string> prefs(branch_preferences);
  std::vector<std::string>::const_iterator prefs_it;
  std::map<std::string,int>::const_iterator f_it;
  
  prefs.push_back(""); /*insert "" branch to search default
			 interface_name too*/
  
  for (prefs_it = prefs.begin();
       prefs_it != prefs.end();
       prefs_it++){
    f_it = Schemaimplementation::info_by_inteface.find(interface_name+*prefs_it);
    if (f_it != Schemaimplementation::info_by_inteface.end())
      return &(Schemaimplementation::class_infos[f_it->second]);
  }
  return 0;
}

void 
Schemaimplementation::register_class(const Schemaimplementation::schemaimplementation_info_t& class_info) throw(){
  MutexLock l(class_info_mutex);

  Schemaimplementation::register_class_i(class_info);
}

void 
Schemaimplementation::register_class_i(const Schemaimplementation::schemaimplementation_info_t& class_info) throw(){
  if (Schemaimplementation::class_info_i(class_info.class_name) != 0) {
    std::cerr << "Schemaimplementation::register_class_i: duplicated class name: " << class_info.class_name << ". Ignored\n";
    return;
  }

  int info_idx = Schemaimplementation::class_infos.size();
  Schemaimplementation::class_infos.push_back(class_info);
  Schemaimplementation::info_by_classname[class_info.class_name] = info_idx;

  std::string s = class_info.interface_name + class_info.implementation_branch;
  if (Schemaimplementation::info_by_inteface.count(s) == 0)
    Schemaimplementation::info_by_inteface[s] = info_idx;

  s = class_info.interface_name;
  /*serch default interface_name info, if not found uses this*/
  if (Schemaimplementation::info_by_inteface.count(s) == 0)
    Schemaimplementation::info_by_inteface[s] = info_idx;
}

Schemaimplementation::schemaimplementation_info_t
Schemaimplementation::parse_class_info(const std::string& class_name,
				       const std::string& file_name) throw(std::runtime_error){
  Schemaimplementation::schemaimplementation_info_t class_info;
  TiXmlDocument doc(file_name.c_str());

  if (!doc.LoadFile())
    throw std::runtime_error("Schemaimplementation::parse_class_info: can't load class info from '" + file_name + "'");

  TiXmlHandle hDoc(&doc);
  TiXmlElement* pElem,*class_infoElem = 0;
  TiXmlHandle hRoot(0);

  pElem = hDoc.FirstChildElement().ToElement();
  if (!pElem)
    throw std::runtime_error("Schemaimplementation::parse_class_info: error parsing class info. Document empty.");

  hRoot = TiXmlHandle(pElem);
  /*search for schemaDef*/
  for( pElem = hRoot.FirstChild("schemaDef").ToElement();
       pElem; 
       pElem=pElem->NextSiblingElement("schemaDef")){
    std::string schemaDef_name(pElem->Attribute("name"));
    if (schemaDef_name == class_name){
      class_infoElem = pElem;
      break;
    }
  }

  if (!class_infoElem)
    throw std::runtime_error("Schemaimplementation::parse_class_info: error parsing class info. Class info not found.");
  
  class_info.class_name = class_name;
  /*interface info*/
  pElem = class_infoElem->FirstChild("interface")->ToElement();
  if (pElem)
    class_info.interface_name = pElem->Attribute("name");
  else
    throw std::runtime_error("Schemaimplementation::parse_class_info: error parsing class info. Schema interface name not found.");

  /*branch*/
  pElem = class_infoElem->FirstChild("branch")->ToElement();
  if (pElem)
    class_info.implementation_branch = std::string(pElem->Attribute("name"));

  /*children*/
  for (pElem = class_infoElem->FirstChild("child")->ToElement();
       pElem;
       pElem = pElem->NextSiblingElement("child")){
    child_info_t child_info;
    TiXmlElement* childElem = pElem->FirstChild("interface")->ToElement();
    if (!childElem)
      throw std::runtime_error("Schemaimplementation::parse_class_info: error parsing class info.Schema child interface name not found.");
    child_info.interface_name = childElem->GetText();

    for ( childElem = pElem->FirstChild("branch")->ToElement();
	  childElem;
	  childElem = childElem->NextSiblingElement("branch"))
      child_info.prefered_branches.push_back(childElem->GetText());

    std::string child_nick(pElem->Attribute("name"));
    if (child_nick.size() == 0)
      child_nick = child_info.interface_name;

    class_info.children_infos[child_nick] = child_info;
  }

  class_info.constructor = 0;
  return class_info;
}


// std::map<std::string, child_info_t> Schemaimplementation::parse_children_info(std::string children_infos){
//   string::size_type start_child_pos = 0,end_child_pos, end_token_pos;
//   std::map<std::string, child_info_t> children;

//   if ( children_infos.size() == 0)
//     return children;

//   end_child_pos = children_info.find(';',start_child_pos);
//   if (end_child_pos == string::npos)
//     end_child_pos = children_info.size() -1;

//   while (start_child_pos < children_infos.size()) {
//     end_child_pos = children_info.find(';',start_child_pos);
//     if (end_child_pos == string::npos) /*last child*/
//       end_child_pos = children_info.size() -1;

//     string::size_type child_substr_length = end_child_pos - start_child_pos +1;
//     std::string child_info = children_infos.substr(start_child_pos,child_substr_length);
    

    
  
    

//   }
  
// }
