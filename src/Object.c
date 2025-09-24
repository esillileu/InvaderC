#include "Object.h"

void init_obj(Object2D* obj, short x, short y, Sprite* sprite, Heatmap*heatmap)
{  
    obj->current_frame = 0;
    obj->pos.X = x;
    obj->pos.Y = y;
    obj->sprite = sprite;
    obj->heatmap = heatmap;
}

int check_colide(Object2D* a, Object2D* b)
{
    int x1 = a->pos.X, w1 = a->heatmap->w, y1 = a->pos.Y, h1 = a->heatmap->h;
    int x2 = b->pos.X, w2 = b->heatmap->w, y2 = b->pos.Y, h2 = b->heatmap->h;

    if ( x1 + w1 - 1 < x2 || x2 + w2 - 1 < x1 || y1 + h1 - 1 < y2 || y2 + h2 - 1 < y1) return 0;

    int x0 = (x1 > x2) ? x1 : x2;
    int xE = (x1 + w1 - 1 < x2 + w2 - 1) ? x1 + w1 - 1 : x2 + w2 - 1;   
    int y0 = (y1 > y2) ? y1 : y2;
    int yE = (y1 + h1 - 1 < y2 + h2 - 1) ? y1 + h1 - 1 : y2 + h2 - 1;   

    const unsigned char* A = (const unsigned char*)a->heatmap->data;
    const unsigned char* B = (const unsigned char*)b->heatmap->data;

    for (int y = y0; y <= yE; ++y) 
    {    
        const unsigned char* ra = A + (y - y1) * w1 + (x0 - x1);
        const unsigned char* rb = B + (y - y2) * w2 + (x0 - x2);
        for (int i = 0, n = xE - x0 + 1; i < n; ++i) 
            if (ra[i] & rb[i]) return 1;
    }
    return 0;
}

Object2D create_obj(short x, short y, Sprite* sprite, Heatmap* heatmap)
{
    Object2D obj;
    obj.current_frame = 0;
    obj.pos.X = x;
    obj.pos.Y = y;
    obj.sprite = sprite;
    obj.heatmap = heatmap;
    return obj;
}
