#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

class Site;

class SiteCollection {
public:
  void addSite(std::string name, SiteCollection* sites);
  void caluculateScore();
  Site* getNeighbor(std::string key);
  void makeSiteCollection(std::ifstream &ifs, SiteCollection* sites);
  void printScores();
private:
  std::map<std::string, Site*> m_sites;
};

class Site {
public:
  Site(std::string name, SiteCollection* sites):
    m_name(name), m_sites(sites) { };
  
  void storeSiteData(std::string link) {
    m_link_to.push_back(link);
  }

  void passScoreToNeighbor() {
    double average = m_score / m_link_to.size();
    for (auto &key : m_link_to) {
    m_sites->getNeighbor(key)->m_new_score += average;
    }
  }
  
  void update() {
    m_score = m_new_score;
    m_new_score = 0;
  }

  void printScore() {
    std::cout << m_name + " " << m_score << std::endl;
  }
  
private:
  std::string m_name;
  double m_score = 100;
  double m_new_score = 0;
  std::vector<std::string> m_link_to;
  SiteCollection* m_sites;
};

void SiteCollection::addSite(std::string name, SiteCollection* sites) {
  m_sites[name] = new Site(name, sites);
}

void SiteCollection::caluculateScore() {
  for (auto site : m_sites) {
    site.second->passScoreToNeighbor();
  }
  for (auto site : m_sites) {
    site.second->update();
  }
}

Site* SiteCollection::getNeighbor(std::string key) {
  return m_sites[key];
}

void SiteCollection::makeSiteCollection(
  std::ifstream &ifs, SiteCollection* sites) {
  std::string line, key,link;
  getline(ifs, line);
  int sites_num = std::stod(line, nullptr);
  for (int i = 0; i < sites_num; i++) {
    getline(ifs, line);
    addSite(line, sites);
  }
  getline(ifs, line);
  int link_num = std::stod(line, nullptr);
  for (int i = 0; i < link_num; i++) {
    getline(ifs, key, ' ');
    getline(ifs, link);
    m_sites[key]->storeSiteData(link);
  }
}

void SiteCollection::printScores() {
  for (auto site : m_sites) {
    site.second->printScore();
  }
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
  SiteCollection sites;
  sites.makeSiteCollection(ifs, &sites);
  for (int i = 0; i < 30; i++) {
    sites.caluculateScore();  
  }
  sites.printScores();
  return 0;
}
