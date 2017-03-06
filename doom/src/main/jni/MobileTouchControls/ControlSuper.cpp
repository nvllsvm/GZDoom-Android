#include "ControlSuper.h"
#include "TouchControls.h"

using namespace touchcontrols;

ControlSuper::ControlSuper(int type_,std::string t,RectF pos)
{
	controlPos = pos;
	controlPos.left /= ScaleX;
	controlPos.right /= ScaleX;
	controlPos.top /= ScaleY;
	controlPos.bottom /= ScaleY;

	type = type_;
	tag = t;
	enabled = true;
	hidden = false;
}
void ControlSuper::setEnabled(bool v)
{
	enabled = v;
}

void ControlSuper::setHidden(bool v)
{
	hidden = v;
}

bool ControlSuper::isEnabled()
{
	return enabled;
}
bool ControlSuper::isHidden()
{
	return hidden;
}

void ControlSuper::saveXML(TiXmlElement &root)
{
	root.SetAttribute("enabled",enabled);
	root.SetAttribute("hidden",hidden);
	root.SetDoubleAttribute("left",controlPos.left);
	root.SetDoubleAttribute("right",controlPos.right);
	root.SetDoubleAttribute("top",controlPos.top);
	root.SetDoubleAttribute("bottom",controlPos.bottom);

}


void ControlSuper::loadXML(TiXmlElement &root)
{
	root.QueryBoolAttribute("enabled",&enabled);
	root.QueryBoolAttribute("hidden",&hidden);
	root.QueryFloatAttribute("left",  &controlPos.left);
	root.QueryFloatAttribute("right", &controlPos.right);
	root.QueryFloatAttribute("top",   &controlPos.top);
	root.QueryFloatAttribute("bottom",&controlPos.bottom);

}
