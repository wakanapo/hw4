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

std::unordered_map<std::string, Node*> set_data(std::ifstream &ifs) {
  std::string line, key, link;
  std::unordered_map<std::string, Node*> node_map;
  getline(ifs, line);
  int node_num = std::stod(line, nullptr);
  for (int i = 0; i < node_num; i++) {
    getline(ifs, line);
    node_map[line] = new Node;
  } 
  getline(ifs, line);
  int link_num = std::stod(line, nullptr);
  for (int i = 0; i < link_num; i++) {
    getline(ifs, key, ' ');
    getline(ifs, link);
    node_map[key]->m_link_to.push_back(link);
  }
  return node_map;
}

std::unordered_map<std::string, Node*> calculate_value
(std::unordered_map<std::string, Node*> node_map) {
  for (auto map : node_map) {
    double give_value =
      map.second->m_value / map.second->m_link_to.size();
    for (auto& ref : map.second->m_link_to) {
      node_map[ref]->m_new_value += give_value;
    }
  }
  for (auto map : node_map) {
    map.second->m_value = map.second->m_new_value;
    map.second->m_new_value = 0;
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
  for (auto map : node_map) {
    std::cout << map.first << map.second->m_value << std::endl;
  }
  return 0;
}
