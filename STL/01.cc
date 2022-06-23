#include <algorithm>
#include <vector>

class Widget {};

typedef std::vector<Widget> WidgetContainer;
typedef WidgetContainer::iterator WCIterator;

WidgetContainer cw;
Widget bestWidget;

WCIterator i = std::find(cw.begin(), cw.end(), bestWidget);
