#include "vtkMRMLRegQANode.h"

// VTK includes
#include <vtkCommand.h>
#include <vtkObjectFactory.h>

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLVectorVolumeNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLColorTableNode.h>
#include <vtkMRMLSegmentationNode.h>
#include <vtkMRMLTableNode.h>

//----------------------------------------------------------------------------
static const char* SEGMENTATION_REFERENCE_ROLE = "fixedSegmentationRef";
static const char* REGQA_TABLE_REFERENCE_ROLE = "regQATableRef";
static const char* BACKWARD_REGNODE_REFERENCE_ROLE = "backwardRegNodeRef";
//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLRegQANode);

//----------------------------------------------------------------------------
vtkMRMLRegQANode::vtkMRMLRegQANode() {

	this->VectorVolumeNodeID = NULL;
	this->TransformNodeID = NULL;
	this->VolumeNodeID = NULL;
	this->WarpedVolumeNodeID = NULL;
	this->OutputDirectory = NULL;
        this->SegmentID = NULL;
	this->ROINodeID = NULL;
	this->FiducialNodeID = NULL;
	this->OutputModelNodeID = NULL;

        this->BackwardRegistrationOff();
        this->MovieRunOff();

        this->SegmentID = NULL;

	//Checkerboard Parameters
	this->CheckerboardPattern = 10;
	this->CheckerboardVolumeNodeID = NULL;

	this->FlickerOpacity = 1;

	this->AbsoluteDiffVolumeNodeID = NULL;
	this->JacobianVolumeNodeID = NULL;
	this->InverseConsistVolumeNodeID = NULL;
	
	NumberOfScreenshots = 1;
	
	this->MovieBoxRedState = 0;
	this->MovieBoxYellowState = 0;
	this->MovieBoxGreenState = 0;
        
        this->VolumeColorNodeID = NULL;
        this->WarpedColorNodeID = NULL;


}
//----------------------------------------------------------------------------
vtkMRMLRegQANode::~vtkMRMLRegQANode() {
	this->SetVectorVolumeNodeID(NULL);
	this->SetTransformNodeID(NULL);
	this->SetVolumeNodeID(NULL);
	this->SetWarpedVolumeNodeID(NULL);
	this->SetOutputDirectory(NULL);
	this->SetROINodeID(NULL);
        this->SetSegmentID(NULL);
	this->SetFiducialNodeID(NULL);
	this->SetOutputModelNodeID(NULL);
	this->SetCheckerboardVolumeNodeID(NULL);
	this->SetAbsoluteDiffVolumeNodeID(NULL);
	this->SetJacobianVolumeNodeID(NULL);
        this->SetInverseConsistVolumeNodeID(NULL);
        this->SetVolumeColorNodeID(NULL);
        this->SetWarpedColorNodeID(NULL);
}
//----------------------------------------------------------------------------
bool vtkMRMLRegQANode::ChangeFromBackwardToFoward(){
        

   vtkMRMLRegQANode *backNode = this->GetBackwardRegQAParameters();
   
   if ( backNode == NULL ){
      return false;
   }
   this->DisableModifiedEventOn();
   this->SetOutputDirectory(backNode->GetOutputDirectory());
   this->SetROINodeID(backNode->GetROINodeID());
   this->SetOutputModelNodeID(backNode->GetOutputModelNodeID());
   this->SetAndObserveRegQATableNode(backNode->GetRegQATableNode());

   this->CheckerboardPattern=backNode->CheckerboardPattern;
   this->FlickerOpacity = backNode->FlickerOpacity;

   this->SetInverseConsistVolumeNodeID(backNode->GetInverseConsistVolumeNodeID());
   
   this->MovieBoxRedState = backNode->MovieBoxRedState;
   this->MovieBoxYellowState = backNode->MovieBoxYellowState;
   this->MovieBoxGreenState = backNode->MovieBoxGreenState;

   this->DisableModifiedEventOff();
   this->InvokePendingModifiedEvent();

   return true;
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::ReadXMLAttributes(const char** atts) {
	std::cerr << "Reading RegQA parameter node" << std::endl;
	Superclass::ReadXMLAttributes(atts);

	const char* attName;
	const char* attValue;
	while (*atts != NULL) {
		attName = *(atts++);
		attValue = *(atts++);

		if (!strcmp(attName, "VectorVolumeNodeID")) {
			this->SetVectorVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "TransformNodeID")) {
			this->SetTransformNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "VolumeNodeID")) {
			this->SetVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "WarpedVolumeNodeID")) {
			this->SetWarpedVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "OutputDirectory")) {
			this->SetOutputDirectory(attValue);
			continue;
		}
		if (!strcmp(attName, "ROINodeID")) {
			this->SetROINodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "SegmentID")) {
                        this->SetSegmentID(attValue);
                        continue;
                }
		if (!strcmp(attName, "FiducialNodeID")) {
			this->SetFiducialNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "OutputModelNodeID")) {
			this->SetOutputModelNodeID(attValue);
			continue;
		}
		if (!strcmp(attName, "CheckerboardPattern")) {
			std::stringstream ss;
			ss << attValue;
			ss >> this->CheckerboardPattern;
			continue;
		}
		if (!strcmp(attName,"CheckerboardVolumeNodeID")) {
			this->SetCheckerboardVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName,"FlickerOpacity")) {
			std::stringstream ss;
			ss << attValue;
			ss >> this->FlickerOpacity;
			continue;
		}
		if (!strcmp(attName,"AbsoluteDiffVolumeNodeID")) {
			this->SetAbsoluteDiffVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName,"JacobianVolumeNodeID")) {
			this->SetJacobianVolumeNodeID(attValue);
			continue;
		}
		if (!strcmp(attName,"VolumeColorNodeID")) {
                        this->SetVolumeColorNodeID(attValue);
                        continue;
                }
                if (!strcmp(attName,"WarpedColorNodeID")) {
                        this->SetWarpedColorNodeID(attValue);
                        continue;
                }
	}

	this->WriteXML(std::cout,1);
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::WriteXML(ostream& of, int nIndent) {
	Superclass::WriteXML(of, nIndent);
	vtkIndent indent(nIndent);

	of << indent << " VectorVolumeNodeID=\""
			<< (this->VectorVolumeNodeID ? this->VectorVolumeNodeID : "NULL") << "\"";
	of << indent << " TransformNodeID=\""
			<< (this->TransformNodeID ? this->TransformNodeID : "NULL") << "\"";
	of << indent << " VolumeNodeID=\""
			<< (this->VolumeNodeID ? this->VolumeNodeID : "NULL") << "\"";
	of << indent << " WarpedVolumeNodeID=\""
			<< (this->WarpedVolumeNodeID ? this->WarpedVolumeNodeID : "NULL") << "\"";
	of << indent << " OutputDirectory=\""
			<< (this->OutputDirectory ? this->OutputDirectory : "NULL") << "\"";
	of << indent << " ROINodeID=\""
			<< (this->ROINodeID ? this->ROINodeID : "NULL") << "\"";
        of << indent << " SegmentID=\""
                        << (this->SegmentID ? this->SegmentID : "NULL") << "\"";
	of << indent << " FiducialNodeID=\""
			<< (this->FiducialNodeID ? this->FiducialNodeID : "NULL") << "\"";
	of << indent << " OutputModelNodeID=\""
			<< (this->OutputModelNodeID ? this->OutputModelNodeID : "NULL") << "\"";
	of << indent << " CheckerboardPattern=\"" << this->CheckerboardPattern << "\"";
	of << indent << " CheckerboardVolumeNodeID=\""
			<< (this->CheckerboardVolumeNodeID ? this->CheckerboardVolumeNodeID : "NULL") << "\"";
	of << indent << " AbsoluteDiffVolumeNodeID=\""
			<< (this->AbsoluteDiffVolumeNodeID ? this->AbsoluteDiffVolumeNodeID : "NULL") << "\"";
	of << indent << " JacobianVolumeNodeID=\""
			<< (this->JacobianVolumeNodeID ? this->JacobianVolumeNodeID : "NULL") << "\"";
        of << indent << " VolumeColorNodeID=\""
                        << (this->VolumeColorNodeID ? this->VolumeColorNodeID : "NULL") << "\"";
        of << indent << " WarpedColorNodeID=\""
                        << (this->WarpedColorNodeID ? this->WarpedColorNodeID : "NULL") << "\"";
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::Copy(vtkMRMLNode *anode) {
	Superclass::Copy(anode);
	vtkMRMLRegQANode *node = vtkMRMLRegQANode::SafeDownCast(anode);
	this->DisableModifiedEventOn();

	this->SetVectorVolumeNodeID(node->GetVectorVolumeNodeID());
	this->SetTransformNodeID(node->GetTransformNodeID());
	this->SetVolumeNodeID(node->GetVolumeNodeID());
	this->SetWarpedVolumeNodeID(node->GetWarpedVolumeNodeID());
	this->SetOutputDirectory(node->GetOutputDirectory());
	this->SetROINodeID(node->GetROINodeID());
        this->SetSegmentID(node->GetSegmentID());
	this->SetFiducialNodeID(node->GetFiducialNodeID());
	this->SetOutputModelNodeID(node->GetOutputModelNodeID());

	this->SetCheckerboardVolumeNodeID(node->GetCheckerboardVolumeNodeID());
	this->CheckerboardPattern=node->CheckerboardPattern;
	this->FlickerOpacity = node->FlickerOpacity;
	this->SetAbsoluteDiffVolumeNodeID(node->GetAbsoluteDiffVolumeNodeID());
//
	this->SetJacobianVolumeNodeID(node->GetJacobianVolumeNodeID());
        
        this->MovieBoxRedState = node->MovieBoxRedState;
        this->MovieBoxYellowState = node->MovieBoxYellowState;
        this->MovieBoxGreenState = node->MovieBoxGreenState;
        
        this->SetVolumeColorNodeID(node->GetVolumeColorNodeID());
        this->SetWarpedColorNodeID(node->GetWarpedColorNodeID());

	this->DisableModifiedEventOff();
	this->InvokePendingModifiedEvent();
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::UpdateReferenceID(const char *oldID, const char *newID) {
	if (this->VectorVolumeNodeID && !strcmp(oldID, this->VectorVolumeNodeID)) {
		this->SetAndObserveVectorVolumeNodeID(newID);
	}
	if (this->TransformNodeID && !strcmp(oldID, this->TransformNodeID)) {
		this->SetAndObserveTransformNodeID(newID);
	}
	if (this->OutputDirectory && !strcmp(oldID, this->OutputDirectory)) {
		this->SetAndObserveOutputDirectory(newID);
	}
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveVectorVolumeNodeID(const char* id) {
	if (this->VectorVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->VectorVolumeNodeID, this);
	}
	this->SetVectorVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->VectorVolumeNodeID, this);
	}
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveTransformNodeID(const char* id) {
	if (this->TransformNodeID) {
		this->Scene->RemoveReferencedNodeID(this->TransformNodeID, this);
	}
	this->SetTransformNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->TransformNodeID, this);
	}
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveVolumeNodeID(const char* id) {
	if (this->VolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->VolumeNodeID, this);
	}
	this->SetVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->VolumeNodeID, this);
	}
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveWarpedVolumeNodeID(const char* id) {
	if (this->WarpedVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->WarpedVolumeNodeID, this);
	}
	this->SetWarpedVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->WarpedVolumeNodeID, this);
	}
}
//----------------------------------------------------------------------------
vtkMRMLSegmentationNode* vtkMRMLRegQANode::GetSegmentationNode()
{
  return vtkMRMLSegmentationNode::SafeDownCast( this->GetNodeReference(SEGMENTATION_REFERENCE_ROLE) );
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveSegmentationNode(vtkMRMLSegmentationNode* node)
{
  this->SetNodeReferenceID(SEGMENTATION_REFERENCE_ROLE, (node ? node->GetID() : NULL));
}
//----------------------------------------------------------------------------
vtkMRMLTableNode* vtkMRMLRegQANode::GetRegQATableNode()
{
  return vtkMRMLTableNode::SafeDownCast( this->GetNodeReference(REGQA_TABLE_REFERENCE_ROLE) );
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveRegQATableNode(vtkMRMLTableNode* node)
{
  this->SetNodeReferenceID(REGQA_TABLE_REFERENCE_ROLE, (node ? node->GetID() : NULL));
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveOutputDirectory(const char* id) {
	if (this->OutputDirectory) {
		this->Scene->RemoveReferencedNodeID(this->OutputDirectory, this);
	}
	this->SetOutputDirectory(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->OutputDirectory, this);
	}
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveROINodeID(const char* id) {
	if (this->ROINodeID) {
		this->Scene->RemoveReferencedNodeID(this->ROINodeID, this);
	}
	this->SetROINodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->ROINodeID, this);
	}
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveFiducialNodeID(const char* id) {
	if (this->FiducialNodeID) {
		this->Scene->RemoveReferencedNodeID(this->FiducialNodeID, this);
	}
	this->SetFiducialNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->FiducialNodeID, this);
	}
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveOutputModelNodeID(const char* id) {
        if (this->OutputModelNodeID) {
                this->Scene->RemoveReferencedNodeID(this->OutputModelNodeID, this);
        }
        this->SetOutputModelNodeID(id);

        if (id) {
                this->Scene->AddReferencedNodeID(this->OutputModelNodeID, this);
        }
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveCheckerboardVolumeNodeID(const char* id) {
	if (this->CheckerboardVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->CheckerboardVolumeNodeID, this);
	}
	this->SetCheckerboardVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->CheckerboardVolumeNodeID, this);
	}
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveAbsoluteDiffVolumeNodeID(const char* id) {
	if (this->AbsoluteDiffVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->AbsoluteDiffVolumeNodeID, this);
	}
	this->SetAbsoluteDiffVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->AbsoluteDiffVolumeNodeID, this);
	}
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveJacobianVolumeNodeID(const char* id) {
	if (this->JacobianVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->JacobianVolumeNodeID, this);
	}
	this->SetJacobianVolumeNodeID(id);

	if (id) {
	  this->Scene->AddReferencedNodeID(this->JacobianVolumeNodeID, this);
	}
}

//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveInverseConsistVolumeNodeID(const char* id) {
	if (this->InverseConsistVolumeNodeID) {
		this->Scene->RemoveReferencedNodeID(this->InverseConsistVolumeNodeID, this);
	}
	this->SetInverseConsistVolumeNodeID(id);

	if (id) {
		this->Scene->AddReferencedNodeID(this->InverseConsistVolumeNodeID, this);
	}
}
//----------------------------------------------------------------------------
vtkMRMLRegQANode* vtkMRMLRegQANode::GetBackwardRegQAParameters()
{
  return vtkMRMLRegQANode::SafeDownCast( this->GetNodeReference(BACKWARD_REGNODE_REFERENCE_ROLE) );
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::SetAndObserveBackwardRegQAParameters(vtkMRMLRegQANode* node)
{
  this->SetNodeReferenceID(BACKWARD_REGNODE_REFERENCE_ROLE, (node ? node->GetID() : NULL));
}
//----------------------------------------------------------------------------
void vtkMRMLRegQANode::PrintSelf(ostream& os, vtkIndent indent){
	Superclass::PrintSelf(os,indent);

	os << indent << " VectorVolumeNodeID = "
			<< (this->VectorVolumeNodeID ? this->VectorVolumeNodeID : "NULL") << "\n";
	os << indent << " TransformNodeID = "
			<< (this->TransformNodeID ? this->TransformNodeID : "NULL") << "\n";
	os << indent << " ReferenceNodeID = "
			<< (this->VolumeNodeID ? this->VolumeNodeID : "NULL") << "\n";
	os << indent << " WarpedNodeID = "
			<< (this->WarpedVolumeNodeID ? this->WarpedVolumeNodeID : "NULL") << "\n";
	os << indent << " OutputDirectory = "
			<< (this->OutputDirectory ? this->OutputDirectory : "NULL") << "\n";
	os << indent << " ROINodeID = "
			<< (this->ROINodeID ? this->ROINodeID : "NULL") << "\n";
        os << indent << " SegmentID = "
                        << (this->SegmentID ? this->SegmentID : "NULL") << "\n";
	os << indent << " FiducialNodeID = "
			<< (this->FiducialNodeID ? this->FiducialNodeID : "NULL") << "\n";
	os << indent << " CheckerboardVolumeNodeID = "
			<< (this->CheckerboardVolumeNodeID ? this->CheckerboardVolumeNodeID : "NULL") << "\n";
	os << indent << " CheckerboardPattern = " << this->CheckerboardPattern << "\n";
//
	os << indent << " AbsoluteDiffVolumeNodeID = "
			<< (this->AbsoluteDiffVolumeNodeID ? this->AbsoluteDiffVolumeNodeID : "NULL") << "\n";
	os << indent << " JacobianVolumeNodeID = "
			<< (this->JacobianVolumeNodeID ? this->JacobianVolumeNodeID : "NULL") << "\n";
        os << indent << " InverseConsistVolumeNodeID = "
                        << (this->InverseConsistVolumeNodeID ? this->InverseConsistVolumeNodeID : "NULL") << "\n";
        os << indent << " VolumeColorNodeID = "
                        << (this->VolumeColorNodeID ? this->VolumeColorNodeID : "NULL") << "\n";
        os << indent << " WarpedColorNodeID = "
                        << (this->WarpedColorNodeID ? this->WarpedColorNodeID : "NULL") << "\n";
}
