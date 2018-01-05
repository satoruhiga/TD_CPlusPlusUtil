#pragma once

#include <assert.h>

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "CHOP_CPlusPlusBase.h"
#include "CPlusPlus_Common.h"

class ParManager;

class Par
{
public:

	Par(const std::string& name, const std::string& label = "", const std::string& page = "Custom")
		: name(name)
		, label(label)
		, page(page)
	{}

	template <typename T>
	void build(T& p)
	{
		p.name = this->name.c_str();
		p.label = this->label.c_str();
		p.page = this->page.c_str();
	}

	const std::string& getName() const { return name; }
	const std::string& getLabel() const { return label; }
	const std::string& getPage() const { return page; }

protected:

	friend class ParManager;

	std::string name, label, page;
	
	virtual void update(OP_Inputs* inputs) = 0;
};

//////////////////////////////////////////////////////////////////////////

template <int N>
class NumericPar_ : public Par
{
public:

	using Callback = std::function<void(const std::vector<double>& v)>;

	NumericPar_(const std::string& name, const std::string& label = "", const std::string& page = "Custom")
		: Par(name, label, page)
	{
		value.resize(N);
		build(p);
	}

	void setCallback(Callback callback) {
		this->callback = callback;
	}

	OP_NumericParameter* getParameter() { return &p; }

	size_t size() const { return N; }

protected:

	void update(OP_Inputs* inputs) override
	{
		bool changed = false;

		for (int i = 0; i < value.size(); i++)
		{
			auto v = inputs->getParDouble(name.c_str(), i);
			if (v != value[i])
			{
				value[i] = v;
				changed = true;
			}
		}

		if (changed && callback)
		{
			callback(value);
		}
	}

private:

	friend class ParManager;

	std::vector<double> value;
	Callback callback;

	OP_NumericParameter p;
};

using NumericPar = NumericPar_<1>;

//////////////////////////////////////////////////////////////////////////

class StringPar : public Par
{
public:

	using Callback = std::function<void(const std::string& v)>;

	StringPar(const std::string& name, const std::string& label = "", const std::string& page = "Custom")
		: Par(name, label, page)
	{
		build(p);
	}

	void setCallback(Callback callback) {
		this->callback = callback;
	}

	OP_StringParameter* getParameter() { return &p; }

protected:

	void update(OP_Inputs* inputs) override
	{
		bool changed = false;
		
		const char* v = inputs->getParString(name.c_str());
		if (v != value)
		{
			value = v;
			changed = true;
		}

		if (changed && callback)
		{
			callback(value);
		}
	}

private:

	friend class ParManager;

	std::string value;
	Callback callback;

	OP_StringParameter p;
};

//////////////////////////////////////////////////////////////////////////

class ParManager
{
public:

	void init(OP_ParameterManager* manager);

	template <int N>
	void appendFloat(NumericPar_<N>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendFloat(*p.getParameter(), p.size());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	template <int N>
	void appendInt(NumericPar_<N>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendInt(*p.getParameter(), p.size());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendXY(NumericPar_<2>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendXY(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendXYZ(NumericPar_<3>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendXYZ(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendUV(NumericPar_<2>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendUV(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendUVW(NumericPar_<3>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendUVW(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendRGB(NumericPar_<3>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendRGB(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendRGBA(NumericPar_<4>& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendRGBA(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendToggle(NumericPar& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendToggle(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendPulse(NumericPar& p,
		NumericPar::Callback callback = NumericPar::Callback())
	{
		manager->appendPulse(*p.getParameter());
		p.setCallback(callback);
		pulse_parameters.push_back(&p);
	}

	void appendString(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendString(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendFile(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendFile(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendFolder(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendFolder(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendDAT(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendDAT(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendDAT(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendDAT(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendCHOP(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendCHOP(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendTOP(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendTOP(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	void appendObject(StringPar&p,
		StringPar::Callback callback = StringPar::Callback())
	{
		manager->appendObject(*p.getParameter());
		p.setCallback(callback);
		parameters.push_back(&p);
	}

	//////////////////////////////////////////////////////////////////////////

	void update(OP_Inputs* inputs);
	void pulsePressed(const char* name);

private:

	OP_ParameterManager* manager;
	std::vector<Par*> parameters;
	std::vector<NumericPar*> pulse_parameters;
};