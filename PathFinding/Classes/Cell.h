#ifndef _CELL_H
#define _CELL_H

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

class Cell{
public:
	Cell():_marked(false),_distance(0),_lastX(-1),_lastY(-1),
		_x(-1),_y(-1),_passable(true),_weight(1),_drawProgress(false){
	}

	SYNTHESIZE(int, _x, X);                       //start at left bottom
	SYNTHESIZE(int, _y, Y);                       //start at left bottom
	SYNTHESIZE(int, _lastX, LastX);               //store the nearest cell's location related this cell
	SYNTHESIZE(int, _lastY, LastY);               //store the nearest cell's location related this cell
	SYNTHESIZE(bool, _marked, Marked);            //whether this cell process or not
	SYNTHESIZE(float, _distance, Distance);       //distance between this cell and start
	SYNTHESIZE(bool, _passable, Passable);        //whether this call can pass
	SYNTHESIZE(int, _drawProgress, DrawProgress); //just for draw the path finding progress
	inline void setWeight(int weight){
		if(weight > 4){
			_weight = 1;
		}else{
			_weight = weight;
			setPassable(weight == 4 ? false : true);
		}
		
	}
	inline int getWeight()const{ return _weight;}
private:
	int _weight;              //default is 1, 4 means this cell is impassable.
	                           //distance have relationship with weight
};



#endif