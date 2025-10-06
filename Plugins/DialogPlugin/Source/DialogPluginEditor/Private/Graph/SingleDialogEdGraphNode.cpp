// Copyright pjdevs. All Rights Reserved.


#include "Graph/SingleDialogEdGraphNode.h"


USingleDialogEdGraphNode::USingleDialogEdGraphNode()
	: DialogLineText(FText::FromString("Sample Dialog Line"))
{

}

void USingleDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");
	OutputPin = CreateDialogPin(EGPD_Output,"Next Dialogs");
}
