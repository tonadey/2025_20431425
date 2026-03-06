/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>

#include <vtkDataSetMapper.h>



ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {
    this->actor = nullptr;
    this->reader = nullptr;
    this->mapper = nullptr;

    // Initialize visibility based on the second column of data
    this->isVisible = (data.at(1).toString().toLower() == "true");
    /* You probably want to give the item a default colour */
    // Default / stored colour from columns 2,3,4 if present
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    if (data.size() > 4) {
        r = static_cast<unsigned char>(data.at(2).toInt());
        g = static_cast<unsigned char>(data.at(3).toInt());
        b = static_cast<unsigned char>(data.at(4).toInt());
    }

    colour.Set(r, g, b);
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}


int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}


void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size()) return;
    m_itemData.replace(column, value);

    // If the visibility column (1) is changed, update the internal bool
    if (column == 1) {
        isVisible = (value.toString().toLower() == "true");
    }
}


ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}


int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    colour.Set(R, G, B);
    updateActorProperties();
}

unsigned char ModelPart::getColourR() {
    return colour.GetRed();
}

unsigned char ModelPart::getColourG() {
    return colour.GetGreen();
}


unsigned char ModelPart::getColourB() {
    return colour.GetBlue();
}


void ModelPart::setVisible(bool isVisible) {
    this->isVisible = isVisible;
    this->set(1, isVisible ? "true" : "false");
    updateActorProperties();
}

bool ModelPart::visible() {
    return isVisible;
}

void ModelPart::loadSTL( QString filename ) {
    // 1. Create the reader
    reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(filename.toStdString().c_str());
    reader->Update();

    // 2. Create the mapper
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    // 3. Create the actor
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // Ensure it starts visible
  //  this->isVisible = true;
    // Apply current stored visibility + colour
    updateActorProperties();
}

//Getter for retrieving the actor 
vtkSmartPointer<vtkActor> ModelPart::getActor() {
    return actor;
}

vtkActor* ModelPart::getNewActor() {
  /* 1. Create new mapper */
vtkNew<vtkPolyDataMapper> vrmapper;
vrmapper->SetInputConnection(reader->GetOutputPort());

/* 2. Create new actor and link to mapper */
vtkNew<vtkActor> vractor;
vractor->SetMapper(vrmapper);

/* 3. Link the vtkProperties of the original actor to the new actor. */
// This ensures if you change the color of the part, it changes in VR too!
vractor->SetProperty(actor->GetProperty());

return vractor; }



void ModelPart::updateActorProperties() {
    if (!actor) return;

    // Visibility
    actor->SetVisibility(isVisible ? 1 : 0);

    // Colour
    actor->GetProperty()->SetColor(
        colour.GetRed() / 255.0,
        colour.GetGreen() / 255.0,
        colour.GetBlue() / 255.0
    );
}

