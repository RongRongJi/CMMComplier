#include "treeNode.h"
#include "token.h"


treeNode::treeNode(int type)
{
	m_left = NULL;
	m_right = NULL;
	m_middle = NULL;
	m_next = NULL;
	m_type = type;
	m_lineNo = 0;
	m_dataType = -1;
	switch (m_type) {
	case FACTOR:
		m_dataType = token::PLUS;
		break;
	}
	
}


treeNode::~treeNode()
{
	if (m_left != NULL)
		delete m_left;
	if (m_middle != NULL)
		delete m_middle;
	if (m_right != NULL)
		delete m_right;
}

treeNode* treeNode::getLeft() {
	return m_left;
}
void treeNode::setLeft(treeNode* left) {
	if (m_left != NULL)
		delete m_left;
	m_left = left;
}

treeNode * treeNode::getMiddle()
{
	return m_middle;
}

void treeNode::setMiddle(treeNode * middle)
{
	if (m_middle != NULL)
		delete m_middle;
	m_middle = middle;
}

treeNode * treeNode::getRight()
{
	return m_right;
}

void treeNode::setRight(treeNode * right)
{
	if (m_right != NULL)
		delete m_right;
	m_right = right;
}

treeNode * treeNode::getNext()
{
	return m_next;
}

void treeNode::setNext(treeNode * next)
{
	m_next = next;
}

int treeNode::getType()
{
	return m_type;
}

void treeNode::setType(int type)
{
	m_type = type;
}

int treeNode::getDataType()
{
	return m_dataType;
}

void treeNode::setDataType(int dataType)
{
	m_dataType = dataType;
}

std::string treeNode::getValue()
{
	return m_value;
}

void treeNode::setValue(const std::string & value)
{
	m_value = value;
}

int treeNode::getLineNo()
{
	return m_lineNo;
}

void treeNode::setLineNo(int lineNo)
{
	m_lineNo = lineNo;
}

string treeNode::toString() {
	switch (m_type) {
	case NUL:
		return "NULL";
	case IF_STMT:
		return "IF_STMT";
	case WHILE_STMT:
		return "WHILE_STMT";
	case FOR_STMT:
		return "FOR_STMT";
	case READ_STMT:
		return "READ_STMT";
	case WRITE_STMT:
		return "WRITE_STMT";
	case DECLARE_STMT:
		return "DECLARE_STMT";
	case ASSIGN_STMT:
		return "ASSIGN_STMT";
	case STMT_BLOCK:
		return "STMT_BLOCK";
	case RETURN_STMT:
		return "RETURN_STMT";
	case EXP:
		return "EXP";
	case ADDTIVE_EXP:
		return "ADDTIVE_EXP";
	case TERM:
		return "TERM";
	case VAR:
		return "VAR";
	case FUNDECLARE:
		return "FUNDECLARE";
	case FUNCALL:
		return "FUNCALL";
	case OP:
		return token::typeToString(this->getDataType());
		//return "OP";
	case FACTOR:
		return "FACTOR";
	case LITERAL:
		return "LITERAL";
	case REG:
		return "REG";
	}
	return nullptr;
}

 void treeNode::travelTree(treeNode * node,int blk) {
	 cout << node->toString() << " " << endl;
	 if (node->getLeft()) {
		 for (int i = 0; i < blk; i++) cout << "--";
		 cout << "left: " ; travelTree(node->getLeft(), blk + 1);
	 }
	 if (node->getMiddle()) {
		 for (int i = 0; i < blk; i++) cout << "--";
		 cout << "middle: " ; travelTree(node->getMiddle(), blk + 1);
	 }
	 if (node->getRight()) {
		 for (int i = 0; i < blk; i++) cout << "--";
		 cout << "right: " ; travelTree(node->getRight(), blk + 1);
	 }
	 if (node->getNext()) {
		 for (int i = 0; i < blk; i++) cout << "--";
		 cout << "next: " ; travelTree(node->getNext(), blk + 1);
	 }
	 
}