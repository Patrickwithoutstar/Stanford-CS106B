#include "Plotter.h"
#include "hashmap.h"
#include "vector.h"
#include "strlib.h"
#include "error.h"



using namespace std;


typedef void (*PlotFunc)(Vector<string>);


HashMap<string, PlotFunc> funcMap;

// true -> up ; false -> down;
bool penStatus = false;

Vector<double> position;

PenStyle penStyle = {1, "black"};

string delimiter = " ";



void setPos(double x, double y){
    position.set(0,x);
    position.set(1,y);
}


void moveAbsImpl(double x, double y){
    if(penStatus){
        drawLine(position[0], position[1], x, y, penStyle);
    }
    setPos(x,y);
}

void moveAbs(Vector<string> params){
    moveAbsImpl(stringToReal(params[0]),stringToReal(params[1]));
}

void moveRelImpl(double dx, double dy){

    // draw line

    if(penStatus){
        drawLine(position[0], position[1], position[0]+dx, position[1]+dy, penStyle);
    }

    setPos(position[0] + dx, position[1] + dy);
}

void moveRel(Vector<string> params){
    moveRelImpl(stringToReal(params[0]),stringToReal(params[1]));
}

void penDown(Vector<string> params){
    (void) params;
    penStatus = true;
}

void penUp(Vector<string> params){
    (void) params;
    penStatus = false;
}

void penColor(Vector<string> params){
    penStyle.color = params[0];
}

void penWidth(Vector<string> params){
    penStyle.width = stringToReal(params[0]);
}

void init(){
    string MoveAbs = "moveabs";
    string PenDown = "pendown";
    string MoveRel = "moverel";
    string PenUp = "penup";
    string PenWidth = "penwidth";
    string PenColor = "pencolor";

    funcMap.put(MoveAbs,moveAbs);
    funcMap.put(PenDown,penDown);
    funcMap.put(MoveRel,moveRel);
    funcMap.put(PenUp,penUp);
    funcMap.put(PenColor,penColor);
    funcMap.put(PenWidth,penWidth);

    penStatus =false;
    position = {0, 0};
    penStyle = { 1, "black" };
}

void execute(string order, Vector<string> params){
    PlotFunc func = funcMap.get(toLowerCase(order));
    if(func==0) {
        string msg = "Invalid operation ***"+order+"*** in this plot";
        error(msg);
    }
    func(params);
    return;
}

void runPlotterScript(istream& input) {
    init();

    for (string line; getline(input, line); ) {
        /* ... do something with line ... */
        Vector<string> strv = stringSplit(line, delimiter);
        std::cout << strv << endl;
        string order = strv[0];
        Vector<string> params= strv.subList(1);
        execute(order, params);
    }
}




