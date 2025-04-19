#ifndef __WNDPREVIEWCHAO_H__
#define __WNDPREVIEWCHAO_H__
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/box2d.h>
#include "env.h"

class CPoint
{
public:
    CPoint(const int x, const int y): X(0), Y(0)
    {
        X = x;
        Y = y;
    }

    CPoint(): X(0), Y(0) {}
    int X;
    int Y;
};

namespace GUI
{
    class CWndPreviewChao final
    {
        sf::RenderWindow m_window;

    public:
        CWndPreviewChao();

        virtual ~CWndPreviewChao();

        void SetCenter(int x, int y);

        void show();

        void flush();

        void Invalidate() {}

        MODEL::CEnv m_env     ;
        b2Vec2      m_vecTl   ;
        b2Vec2      m_vecBr   ;
        CPoint      m_ptCenter;
        int         m_zoom    ;

        void OnPaint();
    };
}
#endif //__WNDPREVIEWCHAO_H__
