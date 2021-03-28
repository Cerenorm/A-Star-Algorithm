#include <stdio.h>
#include <stdlib.h>

#define MAP_WIDTH  20
#define MAP_HEIGHT 20


int mapf[MAP_WIDTH*MAP_HEIGHT]=
{
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,
    0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
    0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,
    0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0
};


//Baðlý liste düðüm yapýsý
typedef struct POINT
{
    int x;
    int y;
    struct POINT *next;
} POINT;


/*
Harita uzerinde baslangic ve bitis noktalari arasindaki en kisa yolu bulur.
src_x,src_y: Baslangic koordinatlari
dst_x,dst_y: Bitis koordinatlari
map_w,map_h: Harita genisligi ve yuksekligi
Fonksiyonun donus degeri bagli liste olarak yol koordinatlari
Yol bulunamazsa donus degeri NULL
*/
POINT * AStar(int * map,int map_w,int map_h,int src_x, int src_y,int dst_x,int dst_y)
{
    int *tx,*ty;
    int i,j;
    int cx,cy;
    int cur,end;
    int len;
    int value;
    POINT * cp;
    POINT * pt=NULL;
    POINT * last=NULL;

    len=map_w*map_h;

    tx=(int *)malloc(len*sizeof(int));
    ty=(int *)malloc(len*sizeof(int));
    tx[0]=src_x;
    ty[0]=src_y;
    map[src_y*map_w +src_x]=10;
    //cur: kontrol edilen nokta
    //end: kontrol edilmemiþ noktalar
    //Geçerli bütün noktalar kontrol edildiðinde cur ve end deðiþkenleri
    //ayný olacak.(Bu gerçekleþtiðinde yol bulunamamýþ demektir.)
    end=1;
    cur=0;
    while(cur<end)
    {
        cx=tx[cur];
        cy=ty[cur];
        if(cx==dst_x && cy==dst_y)
        {
            //Bitiþ noktasýna ulaþýldý yol bulundu, döngüden çýkýlýyor.
            break;
        }
        value=map[cy*map_w +cx];
        cur++;
        //Noktanýn dört tarafýndaki noktalar kontrol ediliyor.
        for(i=cy-1; i<=cy+1; i++)
        {
            for(j=cx-1; j<=cx+1; j++)
            {
                //i,j koordinatlarýnýn harita sýnýrlarý içinde olduðu kontrol ediliyor.
                if(i<map_h && i>=0 && j<map_w && j>=0)
                {
                    //i,j noktasýnýn etrafýndaki 4 dik nokta (çapraz noktalar hariç)
                    if ((i-cy==0 && j-cx)||(j-cx==0 && i-cy))
                    {
                        //Kontrol edilen nokta boþ ise deðeri deðiþtiriliyor.
                        if(map[i*map_w +j]==0)
                        {
                            map[i*map_w +j]=value+10;
                            tx[end]=j;
                            ty[end]=i;
                            end++;
                        }
                    }
                }
            }
        }
    }

    //Yol bulunamadý, ayrýlmýþ bellek boþaltýlýyor ve NULL deðeri ile fonksiyondan çýkýlýyor.
    if(cur>=end)
    {
        free(tx);
        free(ty);
        return NULL;
    }

    /*
    Bulunan yolun koordinatlarý sýra ile sondan baþlayarak baþa doðru kaydediliyor.
    Son noktadan baþlayarak, noktanýn etrafýndaki en küçük deðerli noktaya geçiliyor
    daha sonra bu noktanýn etrafýndaki en küçük deðerli noktaya gidiliyor, bu iþlem
    baþlangýç noktasýna (deðeri 10 olan noktaya) ulaþýncaya kadar devam ediyor. Gidilen
    her noktanýn koordinatlarý kaydediliyor ve sonuçta bu koordinatlar baþlangýç ve bitiþ
    arasýndaki yolu oluþturuyor.
    */
    end=0;
    int lv=map[cy*map_w+cx];
    int change=0;
    while(lv>10)
    {
        change=0;
        for(i=cy-1; i<=cy+1; i++)
        {
            for(j=cx-1; j<=cx+1; j++)
            {
                if((i-cy && j-cx))
                {
                    continue;
                }
                if(i>=0 && i<map_h && j>=0 && j<map_w)
                {
                    if(map[i*map_w+j]<lv && map[i*map_w+j]>1)
                    {
                        lv=map[i*map_w+j];
                        tx[end]=j;
                        ty[end]=i;
                        change=1;
                    }
                }
            }
        }
        if(change)
        {
            cx=tx[end];
            cy=ty[end];
            end++;
        }
    }

    //Yolun koordinatlarý baðlý listeye kaydediliyor.(Sondan baþa doðru)
    end--;
    while(end>=0)
    {
        cp=(POINT *)malloc(sizeof(POINT));
        cp->x=tx[end];
        cp->y=ty[end];
        cp->next=NULL;
        if(pt){
            last->next=cp;
            last=cp;
        }else{
            pt=cp;
            last=cp;
        }
        end--;
    }


    free(tx);
    free(ty);
    return pt;
}





int main()
{
    int i=0,j=0;
    POINT* n=NULL;
    POINT* cp;
    unsigned char block=219;



    //Baþlangýç ve bitiþ noktalarý arasýndaki yol bulunuyor.
    cp=n=AStar(mapf,MAP_WIDTH,MAP_HEIGHT,1,1,8,19);

    //Yol koordinatlarý haritaya kaydediliyor.
    while(n)
    {
        mapf[n->y*MAP_WIDTH+n->x]=0xFFFFFFFF;
        n=n->next;
    }

    //Harita çerçevesinin üst bloðu
    for(j=0;j<MAP_WIDTH+1;j++){
        printf("%c%c",block,block);
    }
    printf("\n");

    //Harita içeriðinin çizimi
    for(i=0;i<MAP_HEIGHT;i++){
        printf("%c",block);
        for(j=0;j<MAP_WIDTH;j++){
            if(mapf[i*MAP_WIDTH + j]==0xFFFFFFFF){
                printf("x ");
            }else if(mapf[i*MAP_WIDTH + j]==1){
                printf("%c%c",block,block);
            }else{
                printf("  ");
            }
        }
        printf("%c\n",block);
    }

    //Harita çerçevesinin alt bloðu
    for(j=0;j<MAP_WIDTH+1;j++){
        printf("%c%c",block,block);
    }
    printf("\n");

    //Baðlý liste siliniyor.
    n=cp;
    while(cp){
        cp=n->next;
        free(n);
        n=cp;
    }

    return 0;
}




















