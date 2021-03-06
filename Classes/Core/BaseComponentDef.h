#pragma once

#define CREATE_COMPONENT_FUNC(__TYPE__) \
static __TYPE__* create(DlgBase* dlg) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(dlg)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}