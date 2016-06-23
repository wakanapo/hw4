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
  static SiteCollection *createFromFile(std::string filename);
  void printScores();
private:
  std::map<std::string, Site*> m_sites;
};

class Site {
public:
  Site(std::string name): m_name(name) { };
  
  void linkTo(Site *site) {
    m_link_to.push_back(site);
  }

  void passScoreToNeighbor() {
    double average = m_score / m_link_to.size();
    for (auto site : m_link_to) 
      site->m_new_score += average;
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
};

void SiteCollection::addSite(std::string name) {
  m_sites[name] = new Site(name);
}

void SiteCollection::caluculateScore() {
  for (auto site : m_sites) 
    site.second->passScoreToNeighbor();
  for (auto site : m_sites) 
    site.second->update();
}

SiteCollection* SiteCollection::createFromFile(std::string filename) {
  std::string line, key, link;
  SiteCollection* sites = new SiteCollection;
  std::ifstream ifs(filename);
  if (ifs.fail())
    return nullptr;
  getline(ifs, line);
  int sites_num = std::stod(line, nullptr);
  for (int i = 0; i < sites_num; i++) {
    getline(ifs, line);
    sites->addSite(line);
  }
  getline(ifs, line);
  int link_num = std::stod(line, nullptr);
  for (int i = 0; i < link_num; i++) {
    getline(ifs, key, ' ');
    getline(ifs, link);
    sites->m_sites[key]->linkTo(sites->m_sites[link]);
  }
  return sites;
}

void SiteCollection::printScores() {
  for (auto site : m_sites) 
    site.second->printScore();
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    std::cout << "illegal input.\n Example: ./a.out input_file" << std::endl;
  
  SiteCollection *sites = SiteCollection::createFromFile(argv[1]);
  if (!sites)
    return -1;
  for (int i = 0; i < 30; i++) 
    sites->caluculateScore();  
  sites->printScores();
  return 0;
}
