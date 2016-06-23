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
  void addSite(std::string name);
  void caluculateScore();
  Site* getSite(std::string key);
  void makeSiteCollection(std::ifstream &ifs);
  void printScores();
private:
  std::map<std::string, Site*> m_sites;
};

class Site {
public:
  Site(std::string name, SiteCollection* sites):
    m_name(name), m_sites(sites) { };
  
  void storeSiteData(std::string link) {
    m_link_to.push_back(m_sites->getSite(link));
  }

  void passScoreToNeighbor() {
    double average = m_score / m_link_to.size();
    for (auto site : m_link_to) {
      site->m_new_score += average;
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
  std::vector<Site*> m_link_to;
  SiteCollection* m_sites;
};

void SiteCollection::addSite(std::string name) {
  m_sites[name] = new Site(name, this);
}

void SiteCollection::caluculateScore() {
  for (auto site : m_sites) {
    site.second->passScoreToNeighbor();
  }
  for (auto site : m_sites) {
    site.second->update();
  }
}

Site* SiteCollection::getSite(std::string key) {
  return m_sites[key];
}

void SiteCollection::makeSiteCollection(
  std::ifstream &ifs) {
  std::string line, key,link;
  getline(ifs, line);
  int sites_num = std::stod(line, nullptr);
  for (int i = 0; i < sites_num; i++) {
    getline(ifs, line);
    addSite(line);
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
  sites.makeSiteCollection(ifs);
  for (int i = 0; i < 30; i++) {
    sites.caluculateScore();  
  }
  sites.printScores();
  return 0;
}
