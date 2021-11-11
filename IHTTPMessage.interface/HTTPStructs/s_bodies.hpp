#ifndef S_BODIES_HPP
#define S_BODIES_HPP

#include <vector>
#include <string>
#include <iostream>

struct s_bodies {
	std::vector<std::string>	bodies;
	std::string					_full_request;

//	s_bodies(const s_bodies &other) {
//		*this = other;
//	}

//	s_bodies &operator=(const s_bodies &other) {
//		for (const std::string &str : other.bodies) {
//			this->bodies.push_back(str);
//		}
//		return *this;
//	}

	void print() const {
        std::vector<std::string>::const_iterator it = bodies.cbegin();
        std::vector<std::string>::const_iterator ite = bodies.cend();

        for (; it != ite; ++it) {
        std::cout << "|" << (*it);
        }
	}
};

#endif
