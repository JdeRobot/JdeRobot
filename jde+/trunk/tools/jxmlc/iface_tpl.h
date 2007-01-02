#include <jde+.h>
#include <vector>
#include <string>
#include <map>


class <--!IFACE_NAME-->: public Schemainterface {
 public:
  <--!IFACE_NAME-->(const std::string& nick = "<--!IFACE_NAME-->")
    : Schemainterface(<--!IFACE_TYPE-->,"<--!IFACE_NAME-->",nick) {}
  virtual bool has_data_name(const std::string& data_name) const {
    return ((find(perceptions.begin(),perceptions.end(),data_name) != perceptions.end()) or
	    (find(modulations.begin(),modulations.end(),data_name) != modulations.end()));
  }
  virtual std::vector<std::string> const& get_perceptions_names() const {
    return perceptions;
  }

  virtual std::vector<std::string> const& get_modulations_names() const {
    return modulations;
  }
 private:
  /*Data names*/
  static const std::vector<std::string> perceptions;
  static const std::vector<std::string> modulations;
  std::map<std::string,Data> data;

  virtual Data& get_data(const std::string& data_name) {
    return data[data_name];
  }
};
