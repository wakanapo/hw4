#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

class Site;
using SiteCollection = std::map<std::string, Site*>;

class Site {
public:
  explicit Site(SiteCollection *sites) : m_sites(sites) { };
  void setLinkData(std::string key, std::string link) {
    m_sites->at(key)->m_link_to.push_back(link);
  }

  void passScoreToNeighbor() {
    double average = m_score / m_link_to.size();
    for (auto &key : m_link_to)
      m_sites->at(key)->m_new_score += average;
  }

  void update() {
    m_score = m_new_score;
    m_new_score = 0;
  }

  void printScore() {
    for (auto key : *m_sites) {
      std::cout << key.first + " " << key.second->m_score << std::endl;
    }
  }
  
private:
  double m_score = 100;
  double m_new_score = 0;
  std::vector<std::string> m_link_to;
  SiteCollection* m_sites;
};

SiteCollection set_data(std::ifstream &ifs) {
  SiteCollection sites;
  std::string line, key, link;
  getline(ifs, line);
  int sites_num = std::stod(line, nullptr);
  for (int i = 0; i < sites_num; i++) {
    getline(ifs, line);
    sites[line] = new Site(&sites);
  }
  getline(ifs, line);
  int link_num = std::stod(line, nullptr);
  for (int i = 0; i < link_num; i++) {
    getline(ifs, key, ' ');
    getline(ifs, link);
    sites[key]->setLinkData(key, link);
  }
  return sites;
}

SiteCollection calculate_value(SiteCollection sites) {
  for (auto site : sites) {
    site.second->passScoreToNeighbor();
  }
  for (auto site : sites) {
    site.second->update();
  }
  return sites;
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
  SiteCollection sites = set_data(ifs);
  for (int i = 0; i < 30; i++) {
    sites = calculate_value(sites);  
  }
  sites.begin()->second->printScore();
  return 0;
}
