/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qSlicerSubjectHierarchyRegistrationQualityPlugin_h
#define __qSlicerSubjectHierarchyRegistrationQualityPlugin_h

// SlicerRt includes
#include "qSlicerSubjectHierarchyAbstractPlugin.h"
#include "qSlicerRegistrationQualitySubjectHierarchyPluginsExport.h"

// #include "qSlicerDicomRtImportExportSubjectHierarchyPluginsExport.h"

class qSlicerSubjectHierarchyRegistrationQualityPluginPrivate;
class vtkMRMLNode;
class vtkMRMLVolumeNode;
class vtkMRMLAnnotationROINode;
class vtkMRMLMarkupsFiducialNode;
class vtkMRMLScalarVolumeNode;
class vtkMRMLTableNode;
//BTX
/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class Q_SLICER_REGISTRATIONQUALITY_SUBJECT_HIERARCHY_PLUGINS_EXPORT qSlicerSubjectHierarchyRegistrationQualityPlugin : public qSlicerSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qSlicerSubjectHierarchyAbstractPlugin Superclass;
  qSlicerSubjectHierarchyRegistrationQualityPlugin(QObject* parent = NULL);
  virtual ~qSlicerSubjectHierarchyRegistrationQualityPlugin();
 
public:
  /// Determines if a data node can be placed in the hierarchy using the actual plugin,
  /// and gets a confidence value for a certain MRML node (usually the type and possibly attributes are checked).
  /// \param node Node to be added to the hierarchy
  /// \param parentItemID Prospective parent of the node to add.
  ///   Default value is invalid. In that case the parent will be ignored, the confidence numbers are got based on the to-be child node alone.
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   node, and 1 means that the plugin is the only one that can handle the node (by node type or identifier attribute)
//   virtual double canAddNodeToSubjectHierarchy(
//     vtkMRMLNode* node,
//     vtkIdType parentItemID=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID )const;

  /// Determines if the actual plugin can handle a subject hierarchy item. The plugin with
  /// the highest confidence number will "own" the item in the subject hierarchy (set icon, tooltip,
  /// set context menu etc.)
  /// \param item Item to handle in the subject hierarchy tree
  /// \return Floating point confidence number between 0 and 1, where 0 means that the plugin cannot handle the
  ///   item, and 1 means that the plugin is the only one that can handle the item (by node type or identifier attribute)
  virtual double canOwnSubjectHierarchyItem(vtkIdType itemID)const;

  /// Get role that the plugin assigns to the subject hierarchy item.
  ///   Each plugin should provide only one role.
  Q_INVOKABLE virtual const QString roleForPlugin()const;

  /// Get icon of an owned subject hierarchy item
  /// \return Icon to set, empty icon if nothing to set
  virtual QIcon icon(vtkIdType itemID);

  /// Get visibility icon for a visibility state
  Q_INVOKABLE virtual QIcon visibilityIcon(int visible);

  /// Generate tooltip for an owned subject hierarchy item
  virtual QString tooltip(vtkIdType itemID)const;

//   /// Set display visibility of a owned subject hierarchy item
//   Q_INVOKABLE virtual void setDisplayVisibility(vtkIdType itemID, int visible);
// 
//   /// Get display visibility of a owned subject hierarchy item
//   /// \return Display visibility (0: hidden, 1: shown, 2: partially shown)
//   Q_INVOKABLE virtual int getDisplayVisibility(vtkIdType itemID)const;

  /// Get item context menu item actions to add to tree view
  Q_INVOKABLE virtual QList<QAction*> itemContextMenuActions()const;

  /// Show context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the context menu items for
  Q_INVOKABLE virtual void showContextMenuActionsForItem(vtkIdType itemID);

protected slots:
  /// Add current segment as a basis for DIRQA
  
  /// Load volume from filename
  void loadFromFilenameForCurrentID();
  vtkMRMLVolumeNode* loadVolumeFromItemId(vtkIdType itemID);
  void calcuateDIRQAForCurrentNode();
  void calcuateDIRQAForID(vtkIdType itemID, bool removeNodes);
  void updateRegNodeForCurrentNode();
  void updateRegNodeForID(vtkIdType itemID);
  vtkMRMLAnnotationROINode* loadROI(vtkIdType itemID);
  vtkMRMLMarkupsFiducialNode* loadMarkups(vtkIdType itemID);
  void calculateAbsoluteDifference(vtkIdType itemID, bool removeNodes);
  void calculateJacobian(vtkIdType itemID, bool removeNodes);
  void calculateInverseConsistency(vtkIdType itemID, bool removeNodes);
  void calculateFiducialDistance(vtkIdType itemID, bool removeNodes);
  vtkIdType findInverseVectorID(vtkIdType itemID);
//   void calculateStatistics(vtkMRMLScalarVolumeNode* scalarNode, double statisticValues[4]);
  void removeNode(vtkMRMLNode* node);
  void writeInTable(vtkIdType itemID,char const* tableName, double* statisticValues, vtkMRMLMarkupsFiducialNode* fiducalsNode = NULL);
  vtkMRMLTableNode* createTable(vtkIdType itemID,char const* tableName, bool fiducalsTable = false);
  
  /// Functions to add appropriate nodes to RegQA module
  void ScalarImageSelected(const char* name);
  
  void fixedImageSelected();
  void movingImageSelected();
  void fwarpedImageSelected();
  void bwarpedImageSelected();
  void fixedVectorFieldSelected();
  void movingVectorFieldSelected();
  void fixedFiducialsSelected();
  void movingFiducialsSelected();
  void ROISelected();
  void fixedContourSelected();
  void movingContourSelected();

  void setNodeInRegQA(vtkMRMLNode* node, QString role, QString context = "");

protected:  
  vtkIdType m_SelectSegment;
protected:
  QScopedPointer<qSlicerSubjectHierarchyRegistrationQualityPluginPrivate> d_ptr;
  

private:
  Q_DECLARE_PRIVATE(qSlicerSubjectHierarchyRegistrationQualityPlugin);
  Q_DISABLE_COPY(qSlicerSubjectHierarchyRegistrationQualityPlugin);
};
//ETX
#endif