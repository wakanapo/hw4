#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Node {
  double m_value = 100;
  double m_new_value = 0;
  std::vector<std::string> m_link_to;
};

int node_num, link_num;

std::unordered_map<std::string, Node*> set_data (std::ifstream &ifs) {  
  std::string line, key, link;
  std::unordered_map<std::string, Node*> node_map;
  getline(ifs, line);
  node_num = std::stod(line, nullptr);
  for (int i = 0; i < node_num; i++) {
    getline(ifs, line);
    node_map[line] = new Node;
  }
  
  getline(ifs, line);
  link_num = std::stod(line, nullptr);
   for (int i = 0; i < link_num; i++) {
     getline(ifs, key, ' ');
     getline(ifs, link);
    node_map[key]->m_link_to.push_back(link);
  }
   return node_map;
}

std::unordered_map<std::string, Node*> calculate_value
(std::unordered_map<std::string, Node*> node_map) {
  for (auto it = node_map.begin(); it != node_map.end(); it++) {
    double give_value =
      node_map[it->first]->m_value / node_map[it->first]->m_link_to.size();
    for (int i = 0; i < (int)node_map[it->first]->m_link_to.size(); i++) {
      std::string key = node_map[it->first]->m_link_to[i];
      node_map[key]->m_new_value += give_value;
    }
  }
  for (auto it = node_map.begin(); it != node_map.end(); it++) {
    node_map[it->first]->m_value = node_map[it->first]->m_new_value;
    node_map[it->first]->m_new_value = 0;
  }
  return node_map;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "illegal input.\n Example: ./a.out input_file" << std::endl;
  }
  std::ifstream ifs(argv[1]);
  if (ifs.fail()) {
    std::cerr << "error" << std::endl;
    return -1;
  }
  std::unordered_map<std::string, Node*> node_map = set_data(ifs);
  for (int i = 0; i < 30; i++) {
    node_map = calculate_value(node_map);  
  }
   for (auto it = node_map.begin(); it != node_map.end(); ++it) {
     std::cout << it->first << node_map[it->first]->m_value << std::endl;
   }
   return 0;
}
