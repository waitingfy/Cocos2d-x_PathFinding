#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <vector>

using namespace std;


template <class Datatype>
class Array2D{

public:
	Array2D(int p_width, int p_height):m_array(p_width * p_height),
		m_width(p_width),m_height(p_height){
	}

	Datatype* Get(int p_x, int p_y)const{
		return m_array[p_y * m_width + p_x];
	}

	void Set(int p_x, int p_y, Datatype* data){
		m_array[p_y * m_width + p_x] = data;
	}

	int Size() const{
		return m_width * m_height;
	}

	int Width() const{
		return m_width;
	}

	int Height()const{
		return m_height;
	}
private:

	vector<Datatype*> m_array;
	int m_width;
	int m_height;

};




#endif