/****************************************************************************
 Copyright (c) 2015-2016 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include <string>
#include "2d/CCComponent.h"

NS_CC_BEGIN

class ComponentJS : public Component
{
public:
    static ComponentJS* create(const std::string& scriptFileName);

    ComponentJS(const std::string& scriptFileName);
    ~ComponentJS();

    /**
     * This function is used to be invoked from JS side to get the corresponding script object of this component.
     */
    void* getScriptObject() const;

    virtual void update(float dt);
    virtual void onEnter();
    virtual void onExit();

private:
    // Script file path
    std::string _scriptFileName;

    // The returned value from require
    void* _jsObj;

    bool _succeedLoadingScript;

    static const std::string ON_ENTER;
    static const std::string ON_EXIT;
    static const std::string UPDATE;
};

NS_CC_END

