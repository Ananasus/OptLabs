/***
* @file tree.h
* @author  FakeEmperor <fake_empire@mail.ru>
* @version 1.0
* @link http://github.com/fakeemperor/TMP-LAB04/
*
* @section DESCRIPTION
*
* The TreeNode storaging class is defined and implemented here
*
*/
#pragma once
#include <sstream>
#include <string>
#include <ostream>

/**
*	Enumeration that is used to define walk method for some functions in <I>TreeNode</I> class.
*/
enum WalkMode { 
	/**
	*	Prefix order of walking the tree
	*	(this node, left subtrees, right subtrees)
	*/
	WALK_PRE_ORDER,
	/**
	*	Postfix order of walking the tree
	*	(left subtrees, right subtrees, this node)
	*/
	WALK_POST_ORDER,
	/**
	*	Symmetrical order of walking the tree
	*	(left subtrees, this node, right subtrees)
	*/
	WALK_SYM_ORDER
};

template<typename T>
/***
*	@brief Graph type storaging class.
*	No specific access model is present
*/
class TreeNode {
	/**
	*	Value on that node
	*/
	T value;
	/**
	*	Parent node of this node (can be <B>nullptr</B> if this node is a root node)
	*/
	TreeNode *parent;
	/**
	*	Child nodes of this node (can be <B>nullptr</B> if this node has no child nodes)
	*/
	TreeNode **children;
	/**
	*	Number of children
	*/
	size_t sz;
protected:
	/**
	*	Represents node's value as string.
	*	If there is preconfigured function for representation its value, this function will be called.
	*	Otherwise, a standard output will present.
	*	@return String representation of the value.
	*/
	std::string valToStr(){
		if (tsf)
			return tsf(this->value);
		std::stringstream ss;
		ss << ":val[" << sizeof(T) << " bytes]:";
		return ss.str();
	};
	/**
	*	Prefix-order walk of the tree.
	*	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
	*						 this index will indicate the difference between maximum index in that tree and the defined one.
	*	@param[in,out] str String, that will hold all the elements visited in prefix order.
	*	@return Node by the specified index or <B>nullptr</B>, if out of range.
	*/
	TreeNode* goWalk_pre(size_t &index, std::string *str = nullptr){
		TreeNode* res = nullptr;
		if (str)
			*str += valToStr() + ";";
		if (!index)
			return this;
		else if(this->children){
			size_t i, s;
			s = index > this->sz ? this->sz : index;
			for (i = 0; i < s && index; i++) {
				res = this->children[i]->goWalk_pre(--index, str);
				if (res)
					break;
			}
		}
		return res;
	};
	/**
	*	Postfix-order walk of the tree.
	*	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
	*						 this index will indicate the difference between maximum index in that tree and the defined one.
	*	@param[in,out] str String, that will hold all the elements visited in postfix order.
	*	@return Node by the specified index or <B>nullptr</B>, if out of range.
	*/
	TreeNode* goWalk_post(size_t &index, std::string *str = nullptr){
		TreeNode* res = nullptr;		
		if (this->children&&index){
			size_t i, s;
			s = index > this->sz ? this->sz : index;
			for (i = 0; i < s && index; i++) {
				res = this->children[i]->goWalk_post(--index, str);
				if (res)
					break;
			}
		}
		if (str)
			*str += valToStr() + ";";
		if (!index)
			res=this;
		return res;
	};
	/**
	*	Symmetrical-order walk of the tree.
	*	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
	*						 this index will indicate the difference between maximum index in that tree and the defined one.
	*	@param[in,out] str String, that will hold all the elements visited in symmetrical order.
	*	@return Node by the specified index or <B>nullptr</B>, if out of range.
	*/
	TreeNode* goWalk_sym(size_t &index, std::string *str = nullptr){
		TreeNode* res = nullptr;
		size_t i, s;
		s = this->sz;
		for (i = 0; i < s / 2 + s % 2 && index; i++) {
			index--;
			res = this->children[i]->goWalk_sym(index, str);
			if (res)
				break;
		}
		if (str)
			*str += valToStr() + ";";
		if (!index)
			res = this;

		for (i = s / 2 + s % 2; i < s && index; i++) {
			index--;
			res = this->children[i]->goWalk_sym(index, str);
			if (res)
				break;
		}
		return res;
	};

public:
	/**
	*	Type of a storaged value
	*	@brief Let them people know, in which type cast this thing!
	*/
	typedef T type;
	/**
	*	A function which makes a string representation of holded value.
	*	If this function is a <B>nullptr</B>,
	*	default substitution string will be passed to the <B>TreeNode::toString()</B> method instead.
	*/
	static std::string(*tsf) (const T&);

	TreeNode<T>() : value(){
		this->parent = NULL;
		this->children = NULL;
		this->sz = 0;
	};
	TreeNode<T>(const T &val, TreeNode<T> *parent = NULL, TreeNode<T> *children = NULL, const size_t sz = 0) : value(val){
		this->parent = NULL;
		this->children = NULL;
		this->sz = 0;
		if (parent)
			parent->addChild(*this);
		if (children)
			this->addChildren(children, sz);

		else {
			this->sz = 0;
			this->children = NULL;
		}
	};

	size_t addChild(const TreeNode<T> &nd){
		this->children = (TreeNode <T>**)realloc(this->children, sizeof(TreeNode<T>*)*(++this->sz));
		this->children[this->sz - 1] = new TreeNode<T>(nd.value);
		this->children[this->sz - 1]->parent = this;
		return this->sz;
	};
	size_t addChild(const T &v){
		this->children = (TreeNode <T>**)realloc(this->children, sizeof(TreeNode<T>*)*(++this->sz));
		this->children[this->sz - 1] = new TreeNode<T>(v);
		this->children[this->sz - 1]->parent = this;
		return this->sz;
	};
	size_t addChildren(TreeNode<T> *nds, const size_t num){
		this->children = (TreeNode<T>**)realloc(this->children, sizeof(TreeNode*)*(this->sz += num));
		for (size_t i = 0; i < num; i++){
			this->children[this->sz - num + i] = new TreeNode<T>(nds[i].value);
			this->children[this->sz - num + i]->parent = this;
		}
		return this->sz;
	}

	TreeNode<T>* getChild(const size_t idx){
		if (idx >= this->sz)
			return NULL;
		return this->children[idx];
	};

	TreeNode <T>* removeChild(const size_t idx){
		TreeNode <T>* res;
		if (idx> this->sz)
			return NULL;
		res = this->children[idx];
		for (size_t i = idx; i <sz; i++)
			this->children[i] = this->children[i + 1];
		--this->sz;
		this->children = (TreeNode<T>**)realloc(this->children, sz*sizeof (TreeNode <T>*));
		return res;
	};

	operator T (){
		return this->value;
	};
	T& operator *(){
		return this->value;
	};
	const T& operator *() const{
		return this->value;
	};
	TreeNode<T>& operator [] (const size_t idx){
		size_t ind = idx;
		TreeNode* res = goWalk_pre(ind);
		if (!res)
			throw EINVAL;
		return *res;
	}
	const TreeNode<T>& operator[] (const size_t idx) const{
		size_t ind = idx;
		TreeNode* res = goWalk_pre(ind);
		if (!res)
			throw EINVAL;
		return *res;
	}
	void clear(){
		if (this->children){
			for (size_t i = 0; i < 0; i++) {
				delete this->children[i];
			}
			free(this->children);
			this->children = 0;
		}
		if (this->parent)
			this->parent->removeChild(this->parent->findChild(this));
	};
	~TreeNode <T>(){
		this->clear();
	};
	/**
	*	@brief Searches an index of an element by its node.
	*	@param[in] tn Pointer to that node.
	*	@return Index of an element, or <B>-1</B> if not found
	*/
	size_t findChild(const TreeNode <T> *tn){
		if (tn == nullptr)
			return -1;
		for (size_t I = 0; I < this->sz; I++)
			if (this->children[I]->value == tn->value)
				return I;
		return -1;
	}
	/**
	*	@brief Counts number of direct children in this TreeNode.
	*	@return Number of storaged elements.
	*/
	size_t length() const{
		return this->sz;
	};
	/**
	*	@brief Counts number of <B>ALL</B> elements in the tree.
	*	@return Number of storaged elements.
	*/
	size_t lengthTree() const{
		size_t i, len = 1;
		for (i = 0; this->sz; i++)
			len += this->children[i]->lengthTree();
		return len;
	};
	/**
	*	@brief Represents the current storaging node as std::string value.
	*	@param[in] level Level of the current node for representation.
	*	@return Returns built std::string.
	*/
	std::string toString(const size_t level = 0) const {
		std::stringstream ss;
		size_t i, lvl = level;
		for (i = 0; i <lvl; i++)
			ss << " ";
		if (lvl>0)
			ss << "|-";

		ss << "<" << this->valToStr() << ">" << std::endl;
		++lvl;
		for (i = 0; i< sz; i++)
			ss << this->children[i]->toString(lvl);
		return ss.str();
	}
	/**
	*	Walks the tree and returns the string, containing all elements aligned in specified order.
	*	@param[in] wm Walk order mode
	*	@return Returns built std::string.
	*/
	std::string walkTree(WalkMode wm= WALK_PRE_ORDER){
		std::string str;
		size_t num = -1;
		switch (wm){
		case WALK_PRE_ORDER:
			goWalk_pre(num, &str);
			break;
		case WALK_POST_ORDER:
			goWalk_post(num, &str);
			break;
		case WALK_SYM_ORDER:
			goWalk_sym(num, &str);
			break;
		}
		return str;
	}
	friend std::ostream& operator << (std::ostream &os, const TreeNode<T> &t){
		os << t.toString();
		return os;

	};
};
template<typename T>
std::string(*TreeNode<T>::tsf) (const T&) = nullptr;