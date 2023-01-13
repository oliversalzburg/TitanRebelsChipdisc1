#include "stdafx.h"
#include "Main.h"

//global variables
HINSTANCE			g_hInstance=NULL;
HWND					g_hWnd=NULL;
HWND					g_hD3DWnd=NULL;
CGBassLib*		g_pBassLib=NULL;
CGFont*				g_pMainFont=NULL, *g_pMenuFont=NULL;
int D3DWindowX=0,D3DWindowY=0,D3DWindowW=0,D3DWindowH=0;

HDC			g_hBackgroundDC=NULL;
HBITMAP g_hBackgroundBitmap=NULL;

CGTimer* g_pTimer = NULL;

bool g_bIsMinimized = false;

//d3d shit
LPDIRECT3D9						g_pDirect3DObject=NULL;
LPDIRECT3DDEVICE9			g_pDirect3DDevice=NULL;
D3DPRESENT_PARAMETERS	g_PresentParameters;
bool									g_bFullscreen=false;

CNEMatrix4						g_matProjection;

CNEVector3	g_vPos;
CNEEuler		g_Rot;

LPDIRECT3DSURFACE9			pBackBuffer = NULL;

LPDIRECT3DVERTEXBUFFER9 pBackgroundVB = NULL, pFullscreenVB = NULL;
LPDIRECT3DTEXTURE9			pBackgroundTex = NULL, pBackgroundMaskTex=NULL;

LPDIRECT3DTEXTURE9			pMainFontTex = NULL, pMenuFontTex = NULL;
LPDIRECT3DTEXTURE9			pControlsTex = NULL, pRepeatTex;

LPDIRECT3DVERTEXBUFFER9 pControlsVB=NULL, pRepeatVB=NULL;

LPDIRECT3DVERTEXBUFFER9 g_pCubeVB = NULL;
LPDIRECT3DINDEXBUFFER9	g_pCubeIB = NULL;

HDC											g_hBackgroundOrigDC=NULL;
HBITMAP									g_hBackgroundOrigBitmap=NULL;
HBITMAP									g_hMinimizeCloseBitmap = NULL;
HDC											g_hMinimizeCloseDC = NULL;
HBITMAP									g_hVolumeBitmap = NULL;
HDC											g_hVolumeDC = NULL;
HBITMAP									g_hFontBitmap = NULL;
HDC											g_hFontDC = NULL;
HBITMAP									g_hControlsBitmap = NULL;
HDC											g_hControlsDC = NULL;
HBITMAP									g_hRepeatBitmap = NULL;
HDC											g_hRepeatDC = NULL;

bool g_bMinimizeHoverstate = false, g_bCloseHoverstate = false;
bool g_bControlHoverstate[6] = {false,false,false,false,false};

POINT g_pVolume[] = {	{220,424},{226,424},{221,442},{215,442},
                      {230,424},{235,424},{231,442},{225,442}};

//scenes
CGArray<CGFx*> g_Scenes;
int g_iCurrentScene=1;
bool g_bNextScene=false;
bool g_bScenePlayin=false;

float barvals[7]={0,0,0,0,0,0,0};

//Menu
//New menu+Music Management
struct PlaylistItem {
  CGString Artist, Title;
  CGString Filename;
  CGString TrackNr;
  float xpos, ypos;
  PlaylistItem() { xpos=ypos=0.f; };
};
CGArray<PlaylistItem> g_pPlaylist;
int g_iCurrentTrack=20;
bool g_bRepeat=false;

float g_fTargetY=0.f; //menu scrolling
float g_fScroller2SpeedOffset=0.f; //bottom scroller speed adjustment
float g_fBottomScrollerOffset=0.f;
  

/*##################################################

  ***** ***** ****   ***  *     *     ***** ****
  *     *     *   * *   * *     *     *     *   *
  ***** *     *   * *   * *     *     ***** *   *
      * *     ****  *   * *     *     *     ****
  ***** ***** *   *  ***  ***** ***** ***** *   *

##################################################*/

//bottom:
//alien
const CGString g_pScrollerText20 = "yo, alien here defacing your monitor with some nice words. first of all, great respect for gencha, who didn't end up in an asylum after 40+ interfaces and gui changes. 2nd, thank u for grabbing this cool little co-op release from titan and rebels!! muchos weeks of coding and adjusting the design have gone into this neat little disk... well what is left to say, time for some personal greetings! we start this incredible long list with the fine fruits of titan, thank u for your support, ideas and bug-testing. these include rebb, strobe, svenzzon, rraina, iks, kryps, auld, dionysos, stv, whoopee and the rest of the 40 members i forgot.... respect also goes out to m:et of rebels with his incredible gay design ideas (we are a love team :))... pasy, pohar and trisch + the rest for additional bugtesting and stuff.. the nice rebel musicians who submitted their stuff for this disk.. dalezy, ne7, vincenzo, chromag.. u guys rock! and now onto the traditional greetings... paradox console and paradox pc (apollo, scal, mrnova, hulkstah, wally, satchmo, ninja, duotype, estrayk, and the rest) ami and the tristar posse, int3r and deviancedemo / scoopex / neural (including h20, hitchhikr, stingray, evil, maali, sphinx, and the rest) mups and the ultraelites (sexirantes, s0r[mups], jan[mups], roc[mups] and a bunch of other idiots) the fine guys of shape design (jedi and the rest) ayatollah and warhead of trsi (not to forget irata ofcourse) chumash and the deviants (psychopasswordmomo!) mpd and h20, amrc, reloaded, deviance, all the idiots on pouet.net and last but not least, my girlfriend mieke for sticking up with me and my scenething for the last 14 years, i love u. now vacuum my room. if u kept on reading, well done, heres a last secret. irokos is a fruit. titan4life and speedfisters are idiots. alien signing off.";
//met
const CGString g_pScrollerText21 = "hi m:et of rebels by the keyboard, just to send out the rebels salutes: and, boozing kupoz, brainstorm, conspiracy, cph, demarche, deviance, fresh!mindworks, hardread, keyboarders, mups, outracks, paradox, portalprocess, racers, razor 1911, reloaded, rgba, s!p, scoopex, spaceballs, titan, tristar... and also to all my old mailswapping friends out there!..";
//irokos
const CGString g_pScrollerText22 = "yo, irokos at the keys, i hope you have a good time reading that fucking long scroller, we took some fun in writing it, just to get you mad. anyways, i'm here, listening to the preview number 5678, and i already know you'll love that disk. if you dont, just come to me in a party or something, i'll offer you a beer or more, and you'll tell me that you love it after a while, for sure. only idiots never change, right? random greetings to all the people i've met at parties, on irc and so on.. it would be too long to list them all for sure, but what the hell, let's make it a longer scroller with it! greetings to hoodlum, deviance and rituel :: the bbq under the sun was <3. gargaj, sorry for the last chipdisk scroller shit <3. tristar :: kurvaaaa!1, let's enjoy all our next meetings, you guys are so great. rebels :: too bad i havent seen you guys at breakpoint, let's share some beers at evoke!. paradox :: i owe you some beers guys, keep up the fun, spots <3 <3 <3 !!. danish gold :: i'm so impatient to meet you sphinx bro, let's share some beers at copenhague, and then some days after at evoke too! turtle, you old fart ;). iris :: good to see you guys still kicking some ass!. accession :: greetings to you only anyways, no? please forget everything before and after that greeting!. acid :: radman, wake up ffs! let's meet at assembly!. paradise :: beer had a better taste with you around, oldschool spirit in all its own shape. thanks, from all my heart. orion, osmium :: j'ai faim! Un bbq un de ces 4 dans mon jardin cet ete please :). equinox :: keopsculanus! <3. syn[rj] :: stv, mryoung, kohai, merci pour tout. stv, sorry for having left a real friend alone for a girl who didnt worth it.. antoinette, sale pute, t'as un nom a coucher dehors. plastic :: you guys are great! xen, bonzaj, i wait for the next beer with you guys ;). sac :: you're the kings of the ascii. it was great to meet some of you at different parties, now, i wait for you for the next ones. galm, cro :: <3. dfs :: go to hell. awd :: tr, vouck, i wanna see you more at demoparties for fuck sake! myth and class :: <3. 2am, i dont wanna spend my night in greeting everyone i know. sorry for the forgotten ones, you're in my heart, keep that in mind, even if you dont give a fuck about it. thanks for that coop rebels, let's make some other ones, that'll surely be great as hell. last thing that has to be told : alien is a fruitlicker, and gencha told me he wanted to fuck red, who felt in love of auld recently.. but saida made love with auld, and red got jealous, so 71m took shazz with him to go to mczonk home, so they were able to have a look at that red-saida fight. but deathrow far away in .nz got angry at his bbs, and spammed over the channels that whoopee was a pie, and whoopee got angry at pikzel because of that, who started to beat the shit out of algar and strobe, who were wanking at the same time. kryps came aboard, to look at all of that mess, and started to put his finger in his anus, and rebb got jealous of kryp's anus, so he started to yell at vils, who didnt give a fuck. but bull was hiding behind a tree at this very same time, and kenet shot him with his rifle, directly imported from spain, cause kmuland is in the spanish mafia, and took contact with kenet because of cerror.. lafarge and mindo were watching tv, and discovered that dot and eviljoker were trying to make music together, so paledeth joined them and they had a long talk about why on earth they are in titan. gkmotu came as well, and svenzzon joined him in his trip to the meeting in meat's home. moonlord tried to join, but his train got burned by xerxes, who was a well known terrorist at that time. 4play was idling in front of his amiga, trying to reach deathrow bbs, and di0nysos was eating some merguez. skull, i8u and hobbit joined that great meeting two days later, and kruze 3 days, because he's retarded. pailes, quasimodo and raina were playing with milkytracker at the meeting place, and were surprised to see sergeon, snkorea and spotter coming all together with a fruiticecream in their hands, trying to put it in each other pants. stv and vigo finally arrived one day before the end of the meeting.. the titan history page keeps growing.. take care guys, titan for life!";
//vamecum
const CGString g_pScrollerText23 = "hi, i'm vamecum of rebels. as I am typing, raindrops are pounding the streets outside, and all in all, this day is most suited for sitting in my warm room listening to this chipdisk! i have baked potatoes in the oven, and i will eat them with some butter soon. summer vacation in two weeks, and i'll go home and eat free food. but how will I survive the summer without my pc? with beer, friends and bbq! and hopefully, so will you! dig the tunes in the sun this summer and allow yourself to *freak out* hope to see you at a demoparty in the future  (i.e solskogen :D) oh, personal greetings to #scene.no, mum, dad, brother, kenshi and #edge.   bye!";
//pasy
const CGString g_pScrollerText24 = "yo ppl! pasy is hitting the buttons at the moment. big, big respect to all chipmusicians all over the world. gencha & alien - nice work for making this chipdisk. Big greets to all brothers and sisters ...next please";
//pikzel
const CGString g_pScrollerText25 = "pikzel hammering the keyboard! wowow. have we got some exciting news or what! well, first of all there's the... oh wait. can't really tell you about this. you'll just have to stay with us and pay a little more attention to the scene in the future. we are, no doubt, in great times for scenery. the scene is alive, my friends! love and kisses from me and the other fruits to all of you fellow readers... :)";
//dalezy
const CGString g_pScrollerText26 = "hej hej hemsktmyckethej, dalezy abusing edit.com. this is like the xxth time i write stuff for a production, knowing that i will hate the text as soon as it got out. i have to add that i never write scrolltexts on my own will, but as i'm constantly being treated with getting my tats rubbed off with a tiger's tongue in case i won't obey, i can't easily resist. that sad fact being offered to the wild public, i'll go on with the occasional rubbish. 2 weeks after breakpoint i decided to cut off all my hair for the 6th time, which makes me look like a perfect mix of irokos and m:et now. that sums up to 2 remaining ponytails that i nailed to my door and an afterwards session of intense wondering why i always let my hair grow long like that. in another news, we have a very stupid christian holiday today, that being father's day, a certain holiday where stupid fucktards get drunk under a bridge and cut off their bollocks in order to throw them at random people. hence, germany will die out soon as there's hardly people left who are able to breed. but how can i complain, it's a holiday after all. i'm out of text. greetings to everyone i like. now, go to http://verderben.org, fuckers! - damnlezy out -";
//saida
const CGString g_pScrollerText27 = "saida taking over the scroller... first of all, i wanna thank gencha, alien and all the muscians for this awesome chipdisc! isnt this scroller ever going to end? probably not. after a year of flaming and anger, its nice that it is finally over - make love not war. did you know alien has huge man boobs? and did you know that he has been providing his pets with man boob milk for the last 10 years? in other news: irokos finally found out that he is gay.";
//dionysos
const CGString g_pScrollerText28 = "dionysos on the writing spree here attending the worlds maybe longest scroller, greetings are in no particular order: rioter, d-luxe, loxley & the rest of the old equinox guys. sei, bloody bastard, shaft, punfisher/blackflower.dk, xplicit, xian, illuminati, kingstar, noname and the rest of they guys i forgot along the road. Special shouts goes to: rebels, titan & trsi. Join our channel at irc.efnet.net #titandemo";
//tail
const CGString g_pScrollerTextTail = "...oh btw, red sucks. and kryps should stfu";
CGString g_pScrollerText2;
long g_lScrollerLength;
CGFONTDRAWRESULT g_rBottomScroller={0,0,0.f};

//forward declarations
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int InitD3D(bool fullscreen);
int InitTuneList();
int InitScene();
void MakeSomeLove(float Time);
void MakeSomeGDILove(float Time);
long TgaLoadTGAToTexture(unsigned char*& output, int& width, int& height, const char *filename);

bool g_bForceGDI=false; //force gdi drawing (no d3d)

void ReleaseAll() {
  SAFE_RELEASE(pBackBuffer);

  SAFE_RELEASE(pBackgroundVB);
  SAFE_RELEASE(pFullscreenVB);
  SAFE_RELEASE(pBackgroundTex);
  SAFE_RELEASE(pBackgroundMaskTex);

  SAFE_RELEASE(pMainFontTex);
  SAFE_RELEASE(pMenuFontTex);
  SAFE_RELEASE(pControlsTex);
  SAFE_RELEASE(pRepeatTex);

  SAFE_RELEASE(pControlsVB);
  SAFE_RELEASE(pRepeatVB);

  SAFE_RELEASE(g_pCubeVB);
  SAFE_RELEASE(g_pCubeIB);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);

  D3DWindowX=64;
  D3DWindowY=91;
  D3DWindowW=520;
  D3DWindowH=245;

  //parse parameters
  if (strlen(lpCmdLine)>0) {
    CGString paramstring=lpCmdLine;
    CGArray<CGString> params;
    paramstring.Split(' ',params);
    for (int i=0; i<params.Length(); i++) {
      if (!params[i].Compare("--forcegdi")) {
        g_bForceGDI=true;
      }
    }
  }
  
  MyRegisterClass(hInstance);

  if (!InitInstance (hInstance, nCmdShow))
    return FALSE;

  g_pTimer=new CGTimer();

  //MessageBox(g_hWnd,"trying init3d","info",0);

  MSG Message;
  if (!g_bForceGDI) {
    if (InitD3D(false)) {
      MessageBox(g_hWnd,"unable to init d3d -- falling back to gdi only","error",0);
      g_bForceGDI=true;
      DestroyWindow(g_hD3DWnd);
      g_hD3DWnd=NULL;
    }
  }

  if (InitScene()) {
    MessageBox(g_hWnd,"unable to init scene -- falling back to gdi only","error",0);
    g_bForceGDI=true;
    DestroyWindow(g_hD3DWnd);
    g_hD3DWnd=NULL;
    ReleaseAll();
  }
  
  {
    InitTuneList();

    //build scroller
    g_pScrollerText2.Add(g_pScrollerText20).Add(" ---------- ")
                    .Add(g_pScrollerText21).Add(" ---------- ")
                    .Add(g_pScrollerText22).Add(" ---------- ")
                    .Add(g_pScrollerText23).Add(" ---------- ")
                    .Add(g_pScrollerText24).Add(" ---------- ")
                    .Add(g_pScrollerText25).Add(" ---------- ")
                    .Add(g_pScrollerText26).Add(" ---------- ")
                    .Add(g_pScrollerText27).Add(" ---------- ")
                    .Add(g_pScrollerText28).Add(" ---------- ")
                    .Add(g_pScrollerTextTail);
    
    g_pScrollerText2 = g_pScrollerText2.Lower();
    CGString warning="warning! ";
    warning.Add(CGString((int)g_pScrollerText2.Length()));
    warning.Add(" char long scroller coming up!                         ");
    g_pScrollerText2.Insert(warning);

    g_lScrollerLength = g_pScrollerText2.Length();

    g_pBassLib = new CGBassLib();
    g_pBassLib->InitBass(g_hWnd);
//#ifndef _DEBUG
    g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
    
    PostMessage(g_hWnd,WM_MOUSEMOVE,MK_LBUTTON,MAKELPARAM(350,425));
//#endif

    g_bScenePlayin = true;
    g_pTimer->GetTimeDiff();

    while (true) {
      while(PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
        if (Message.message == WM_QUIT) break;
        TranslateMessage(&Message);
        DispatchMessage(&Message);
      }
      if (Message.message == WM_QUIT) break;
      Sleep(10);

      if (!g_bForceGDI) {
        MakeSomeLove((float)g_pTimer->GetTimeDiff());
      }else{
        MakeSomeGDILove((float)g_pTimer->GetTimeDiff());
      }

      //advance to next track
      QWORD pos = g_pBassLib->GetPosition();
      QWORD len = g_pBassLib->GetLength();
      if (!g_bRepeat && pos >= len)
        SendMessage(g_hWnd,WM_KEYUP,'B',0);
    }
  } 


  SAFE_DELETE(g_pTimer);
  SAFE_DELETE(g_pBassLib);
  
  for (int i=0; i<g_Scenes.Length(); i++)
    delete(g_Scenes[i]);

  g_Scenes.DeleteAllElements();

  SAFE_DELETE(g_pMainFont);
  SAFE_DELETE(g_pMenuFont);

  ReleaseAll();

  SAFE_RELEASE(g_pDirect3DObject);
  SAFE_RELEASE(g_pDirect3DDevice);

  if (g_hMinimizeCloseBitmap!=NULL) {
    DeleteObject(g_hMinimizeCloseBitmap);
    g_hMinimizeCloseBitmap=NULL;
  }
  if (g_hMinimizeCloseDC!=NULL) {
    DeleteDC(g_hMinimizeCloseDC);
    g_hMinimizeCloseDC=NULL;
  }

  if (g_hVolumeBitmap!=NULL) {
    DeleteObject(g_hVolumeBitmap);
    g_hVolumeBitmap=NULL;
  }
  if (g_hVolumeDC!=NULL) {
    DeleteDC(g_hVolumeDC);
    g_hVolumeDC=NULL;
  }

  if (g_hFontBitmap!=NULL) {
    DeleteObject(g_hFontBitmap);
    g_hFontBitmap=NULL;
  }
  if (g_hFontDC!=NULL) {
    DeleteDC(g_hFontDC);
    g_hFontDC=NULL;
  }

  if (g_hControlsBitmap!=NULL) {
    DeleteObject(g_hControlsBitmap);
    g_hControlsBitmap=NULL;
  }
  if (g_hControlsDC!=NULL) {
    DeleteDC(g_hControlsDC);
    g_hControlsDC=NULL;
  }
  if (g_hRepeatBitmap!=NULL) {
    DeleteObject(g_hRepeatBitmap);
    g_hRepeatBitmap=NULL;
  }
  if (g_hRepeatDC!=NULL) {
    DeleteDC(g_hRepeatDC);
    g_hRepeatDC=NULL;
  }

  return (int) Message.wParam;
}

LRESULT CALLBACK D3DWndProc(HWND hWindow,UINT uMessage,WPARAM wparam,LPARAM lparam) {
  if (uMessage == WM_MOUSEMOVE || uMessage == WM_LBUTTONDOWN) {
    int x,y;
    x=LOWORD(lparam)+D3DWindowX;
    y=HIWORD(lparam)+D3DWindowY;
    lparam=MAKELPARAM(x,y);
  }
  if (uMessage != WM_DESTROY)
    return WndProc(hWindow,uMessage,wparam,lparam);
  else
    return DefWindowProc(hWindow,uMessage,wparam,lparam);
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  //create window class for main window
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style				= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= hInstance;
  wcex.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TITANREBELSCHIPDISC1));
  wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+2);
  wcex.lpszMenuName	= NULL;
  wcex.lpszClassName	= "titanrebelschipdisc";
  wcex.hIconSm			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  RegisterClassEx(&wcex);

  if (!g_bForceGDI) {
    //create window class for d3d content
    WNDCLASSEX WindowClass;
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = D3DWndProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.hIcon = NULL;
    WindowClass.hCursor = NULL;
    WindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = "CGD3DWindow";
    WindowClass.hIconSm = NULL;

    return RegisterClassEx(&WindowClass);
  } else {
    return RegisterClassEx(&wcex);
  }
}

int InitTuneList() {
  WIN32_FIND_DATA FileData;
  HANDLE hSearch;
  bool fFinished = false;

  hSearch = FindFirstFile("music/*.*", &FileData); 
  if (hSearch == INVALID_HANDLE_VALUE) return -1; 

  while (!fFinished) {
    if (strcmp(FileData.cFileName,".") != 0 && strcmp(FileData.cFileName,"..") != 0) {
      PlaylistItem* pPlaylistItem = g_pPlaylist.AddElements();
      pPlaylistItem->Filename = "music/";
      pPlaylistItem->Filename.Add(FileData.cFileName);
    }
    
    if (!FindNextFile(hSearch, &FileData)) {
      if (GetLastError() == ERROR_NO_MORE_FILES) { 
        fFinished = true;
      } else { 
        return -1;
      } 
    }
  } 

  FindClose(hSearch);
  g_pPlaylist.Shrink();

  if (g_pPlaylist.Length() == 0) return -2;

  for (int i=0; i<g_pPlaylist.Length(); i++) {
    CGString tmp=CGString(g_pPlaylist[i].Filename);
    tmp=tmp.SubString(tmp.FindCharRev('/')+1);
    tmp=tmp.SubString(0,tmp.FindCharRev('.'));

    g_pPlaylist[i].Artist = tmp.SubString(0,tmp.FindChar('-')-1); 
    g_pPlaylist[i].Artist = g_pPlaylist[i].Artist.SubString(0,tmp.FindCharRev('_')-2).Replace(CGString("_"),CGString(" "));
    g_pPlaylist[i].Title = tmp.SubString(tmp.FindCharRev('-')+2).Replace(CGString("_"),CGString(" "));

    g_pPlaylist[i].TrackNr = i+1;
  }

  return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  g_hInstance = hInstance;

  //create main window
  DWORD dwWindowStyle=WS_OVERLAPPEDWINDOW;
  RECT DesktopSize;
  RECT WindowRect;
  WindowRect.left=0;
  WindowRect.top=0;
  WindowRect.right=640;
  WindowRect.bottom=480;

  AdjustWindowRectEx(&WindowRect,dwWindowStyle,false,0);
  GetClientRect(GetDesktopWindow(),&DesktopSize);

  g_hWnd = CreateWindow("titanrebelschipdisc", "Titan & Rebels Chipdisc #1", dwWindowStyle, 
    (DesktopSize.right / 2 - 320),(DesktopSize.bottom / 2 - 240),WindowRect.right,WindowRect.bottom,
    NULL, NULL, g_hInstance, NULL);

  if (!g_hWnd)
    return FALSE;

  dwWindowStyle &= ~(WS_CAPTION|WS_SIZEBOX);
  SetWindowLong(g_hWnd,GWL_STYLE, dwWindowStyle);
  SetWindowLong(g_hWnd,GWL_EXSTYLE, WS_EX_LAYERED);

  InvalidateRect(g_hWnd, NULL, TRUE);

  SetWindowPos(g_hWnd, NULL, 0, 0, 640, 480, SWP_NOMOVE|SWP_NOZORDER);	

  ShowWindow(g_hWnd, nCmdShow);
  UpdateWindow(g_hWnd);

  if (!g_bForceGDI) {
    //create d3d window
    GetWindowRect(g_hWnd,&WindowRect);
    dwWindowStyle=WS_POPUP;
    WindowRect.left+=D3DWindowX;
    WindowRect.top+=D3DWindowY;
    WindowRect.right=WindowRect.left+D3DWindowW;
    WindowRect.bottom=WindowRect.top+D3DWindowH;

    AdjustWindowRectEx(&WindowRect,dwWindowStyle,false,0);
    
    g_hD3DWnd = CreateWindowEx(WS_EX_TOOLWINDOW,"CGD3DWindow","",dwWindowStyle,
      WindowRect.left,WindowRect.top,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,NULL,NULL,GetModuleHandle(NULL),NULL);

    if (!g_hD3DWnd)
      return FALSE;

    //SetWindowPos(g_hD3DWnd, NULL, 0, 0, D3DWindowW, D3DWindowH, SWP_NOMOVE|SWP_NOZORDER);	
    ShowWindow(g_hD3DWnd, true);
  }
  return TRUE;
}

int InitD3D(bool fullscreen) {
  if (g_hD3DWnd==NULL) return -1;

  if ((g_pDirect3DObject = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
    return -1;
  }
  g_bFullscreen=fullscreen;
  
  ZeroMemory(&g_PresentParameters,sizeof(g_PresentParameters));
  g_PresentParameters.Windowed = true;
  g_PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

  g_PresentParameters.EnableAutoDepthStencil = true;
  g_PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
  g_PresentParameters.hDeviceWindow = g_hD3DWnd;

  g_PresentParameters.BackBufferWidth = D3DWindowW;
  g_PresentParameters.BackBufferHeight = D3DWindowH;
  g_PresentParameters.BackBufferCount = 1;
  g_PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
  g_PresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;

  if(FAILED(g_pDirect3DObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,g_hD3DWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&g_PresentParameters,&g_pDirect3DDevice))) {
    if(FAILED(g_pDirect3DObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,g_hD3DWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_PresentParameters,&g_pDirect3DDevice))) {
      return -1;
    }
  }

  return 0;
}

int InitScene() {
  VOID* pData=NULL;

  if (!g_bForceGDI) {
    float	fAspectRatio,fFieldOfView,fNearPlane,fFarPlane;
    D3DLIGHT9 Light;
    D3DLIGHT9 Light1;
    D3DLIGHT9 Light2;

    fFieldOfView = (float)PI / 6.f;
    fNearPlane = 1.f;
    fFarPlane = 1000.f;
    fAspectRatio = (float)D3DWindowW / (float)D3DWindowH;

    float yscale=.5f/tan(fFieldOfView / 2.f);
    float xscale=fAspectRatio/yscale;
    g_matProjection.m00 = xscale;
    g_matProjection.m01 = 0.0f;
    g_matProjection.m02 = 0.0f;
    g_matProjection.m03 = 0.0f;
    g_matProjection.m10 = 0.0f;
    g_matProjection.m11 = yscale;
    g_matProjection.m12 = 0.0f;
    g_matProjection.m13 = 0.0f;
    g_matProjection.m20 = 0.0f;
    g_matProjection.m21 = 0.0f;
    g_matProjection.m22 = fFarPlane/(fFarPlane-fNearPlane);
    g_matProjection.m23 = 1.0f;
    g_matProjection.m30 = 0.0f;
    g_matProjection.m31 = 0.0f;
    g_matProjection.m32 = -fNearPlane*fFarPlane/(fFarPlane-fNearPlane);
    g_matProjection.m33 = 0.0f;

    g_pDirect3DDevice->SetTransform(D3DTS_PROJECTION,(D3DMATRIX*)&g_matProjection);

    ZeroMemory(&Light,sizeof(Light));
    Light.Type = D3DLIGHT_DIRECTIONAL;
    
    Light.Diffuse.r = .678431f;
    Light.Diffuse.g = .752941f;
    Light.Diffuse.b = .737255f;
    Light.Direction.x = 1.0f;
    Light.Direction.y = 0.0f;
    Light.Direction.z = 1.0f;
    Light.Range = 1000.0f;

    g_pDirect3DDevice->SetLight(0,&Light);
    g_pDirect3DDevice->LightEnable(0,true);

    ZeroMemory(&Light1,sizeof(Light1));
    Light1.Type = D3DLIGHT_POINT;
    
    Light1.Diffuse.r = .415686f;
    Light1.Diffuse.g = .466667f;
    Light1.Diffuse.b = .290196f;
    Light1.Position.x = 0.f;
    Light1.Position.y = 0.f;
    Light1.Position.z = 15.f;
    Light1.Attenuation0 = 0.3f;
    Light1.Range = 10.0f;

    g_pDirect3DDevice->SetLight(1,&Light1);
    g_pDirect3DDevice->LightEnable(1,true);

    ZeroMemory(&Light2,sizeof(Light2));
    Light2.Type = D3DLIGHT_POINT;
    
    Light2.Diffuse.r = 1.0f;
    Light2.Diffuse.g = 1.0f;
    Light2.Diffuse.b = 1.0f;
    Light2.Position.x = 0.f;
    Light2.Position.y = 0.f;
    Light2.Position.z = 0.f;
    Light2.Attenuation0 = 1.0f;
    Light2.Range = 1000.0f;

    g_pDirect3DDevice->SetLight(2,&Light2);
    g_pDirect3DDevice->LightEnable(2,true);

    g_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT,RGB(180,180,180));
    g_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING,false);

    D3DCAPS9 caps;
    g_pDirect3DDevice->GetDeviceCaps(&caps);
    
    for (unsigned int i = 0; i<8; i++) {
      //g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
      
      if (caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFPOINT) {
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_POINT);
      } else {
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
        g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_NONE);
      }

      g_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MAXANISOTROPY,1);
    }
    
  } //if (!g_bForceGDI)

  //init background texture ---------------------------------------------------------------------------------------------
  int width=0,height=0;
  BYTE* TexData=NULL;
  if (TgaLoadTGAToTexture(TexData,width,height,"images/background_flipped.tga")) {
    return -1;
  }

  if (!g_bForceGDI) {
    SAFE_RELEASE(pBackgroundTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pBackgroundTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pBackgroundTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);		
          bytePointer[index]=TexData[j]; //B
          bytePointer[index+1]=TexData[j+1]; //G
          bytePointer[index+2]=TexData[j+2]; //R
          bytePointer[index+3]=TexData[j+3]; //A
          j+=4;
        } 
      }

    pBackgroundTex->UnlockRect(0);
    }
  }//if (!g_bForceGDI) {

  if (g_hBackgroundDC!=NULL) {
    DeleteDC(g_hBackgroundDC);
    g_hBackgroundDC=NULL;
  }
  if (g_hBackgroundBitmap!=NULL) {
    DeleteObject(g_hBackgroundBitmap);
    g_hBackgroundBitmap=NULL;
  }
  g_hBackgroundDC = CreateCompatibleDC(GetDC(NULL));
  g_hBackgroundBitmap = CreateCompatibleBitmap(GetDC(NULL),width,height);
  SelectObject(g_hBackgroundDC,g_hBackgroundBitmap);

  //pre-multiply alpha
  for (int i=0; i<width*height*4; i+=4) {
    TexData[i+0]=TexData[i+0]*TexData[i+3]/255;
    TexData[i+1]=TexData[i+1]*TexData[i+3]/255;
    TexData[i+2]=TexData[i+2]*TexData[i+3]/255;
  }

  if (g_bForceGDI) {
    g_hBackgroundOrigDC = CreateCompatibleDC(GetDC(NULL));
    g_hBackgroundOrigBitmap = CreateBitmap(width,height,1,32,TexData);
    SelectObject(g_hBackgroundOrigDC,g_hBackgroundOrigBitmap);
  }

  LONG retval = SetBitmapBits(g_hBackgroundBitmap,width*height*4,TexData);

  HDC hdcScreen=NULL;
  HDC hdcMemory=NULL;

  hdcScreen = GetDC(NULL);
  hdcMemory = CreateCompatibleDC(hdcScreen);

  HBITMAP oldbmp = (HBITMAP)SelectObject(hdcMemory,g_hBackgroundBitmap);

  RECT windowrect;
  GetWindowRect(g_hWnd,&windowrect);
  POINT windowpos;
  windowpos.x = windowrect.left;
  windowpos.y = windowrect.top;

  SIZE windowsize;
  windowsize.cx = windowrect.right - windowrect.left;
  windowsize.cy = windowrect.bottom - windowrect.top;

  POINT destpos={0,0};

  BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

  BOOL bRet=UpdateLayeredWindow(g_hWnd,hdcScreen,&windowpos,&windowsize,g_hBackgroundDC,&destpos,0,&blendPixelFunction,ULW_ALPHA);
  if (bRet==FALSE) {
    DWORD error=GetLastError();
    return -1;
  }
  
  SelectObject(hdcMemory,oldbmp);
  
  SAFE_FREE(TexData);

  if (!g_bForceGDI) {
    //init background mask ---------------------------------------------------------------------------------------------
    if (TgaLoadTGAToTexture(TexData,width,height,"images/mask_flipped.tga")) {
      return -1;
    }

    SAFE_RELEASE(pBackgroundMaskTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pBackgroundMaskTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pBackgroundMaskTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);		
          bytePointer[index]=TexData[j]; //B
          bytePointer[index+1]=TexData[j]; //G
          bytePointer[index+2]=TexData[j]; //R
          bytePointer[index+3]=TexData[j]; //A
          j+=4;
        } 
      }

    pBackgroundMaskTex->UnlockRect(0);
    }

    SAFE_FREE(TexData);
  }//if (!g_bForceGDI)


  //init background vertex buffer -----------------------------------------------------------------------------------------
  if (!g_bForceGDI) {
    D3DVERTEXORTHO aBackground[4];

    aBackground[0].x = (float)-D3DWindowX;
    aBackground[0].y = (float)-D3DWindowY;
    aBackground[0].z = 0.5f;
    aBackground[0].rhw = 1.0f;
    aBackground[0].u = 0.0f;
    aBackground[0].v = 0.0f;
    
    aBackground[1].x = (float)width-(float)D3DWindowX-1;
    aBackground[1].y = (float)-D3DWindowY;
    aBackground[1].z = 0.5f;
    aBackground[1].rhw = 1.0f;
    aBackground[1].u = 1.0f;
    aBackground[1].v = 0.0f;
    
    aBackground[2].x = (float)width-(float)D3DWindowX-1;
    aBackground[2].y = (float)height-(float)D3DWindowY;
    aBackground[2].z = 0.5f;
    aBackground[2].rhw = 1.0f;
    aBackground[2].u = 1.0f;
    aBackground[2].v = 1.0f;
    
    aBackground[3].x = (float)-D3DWindowX;
    aBackground[3].y = (float)height-(float)D3DWindowY;
    aBackground[3].z = 0.5f;
    aBackground[3].rhw = 1.0f;
    aBackground[3].u = 0.0f;
    aBackground[3].v = 1.0f;
    
    SAFE_RELEASE(pBackgroundVB);
    g_pDirect3DDevice->CreateVertexBuffer(sizeof(aBackground),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEXORTHO,D3DPOOL_MANAGED,&pBackgroundVB,NULL);

    pBackgroundVB->Lock(0,sizeof(pData),(void**)&pData,0);
    memcpy(pData,aBackground,sizeof(aBackground));
    pBackgroundVB->Unlock();
  }//if (!g_bForceGDI) {

  //load menu font --------------------------------------------------------------------------------------------------------
  g_pMenuFont = new CGFont();
  if (g_pMenuFont->LoadFontFromFile(TexData, width, height, "images/font2.bmp"))
    return -1;

  if (!g_bForceGDI) {
    SAFE_RELEASE(pMenuFontTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pMenuFontTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pMenuFontTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);
          if (TexData[j]==0xff) {
            bytePointer[index]=255; //B
            bytePointer[index+1]=255; //G
            bytePointer[index+2]=255; //R
            bytePointer[index+3]=255; //A
          } else {
            bytePointer[index]=0; //B
            bytePointer[index+1]=0; //G
            bytePointer[index+2]=0; //R
            bytePointer[index+3]=0; //A
          }
          j+=4;
        } 
      }

    pMenuFontTex->UnlockRect(0);
    }
  }//if (!g_bForceGDI)
  else {
    g_hFontDC = CreateCompatibleDC(GetDC(NULL));
    g_hFontBitmap = CreateBitmap(width,height,1,32,TexData);
    SelectObject(g_hFontDC,g_hFontBitmap);
  }

  SAFE_FREE(TexData);

  g_pMenuFont->m_CharHeight = 16;
  g_pMenuFont->SetLetter('a',0,0,10);
  g_pMenuFont->SetLetter('b',13,0,10);
  g_pMenuFont->SetLetter('c',26,0,10);
  g_pMenuFont->SetLetter('d',38,0,10);
  g_pMenuFont->SetLetter('e',51,0,10);
  g_pMenuFont->SetLetter('f',63,0,8);
  g_pMenuFont->SetLetter('g',74,0,10);
  g_pMenuFont->SetLetter('h',87,0,10);
  g_pMenuFont->SetLetter('i',98,0,5);
  g_pMenuFont->SetLetter('j',106,0,6);
  g_pMenuFont->SetLetter('k',115,0,10);
  g_pMenuFont->SetLetter('l',128,0,6);
  g_pMenuFont->SetLetter('m',135,0,13);
  g_pMenuFont->SetLetter('n',150,0,10);
  g_pMenuFont->SetLetter('o',163,0,10);
  g_pMenuFont->SetLetter('p',176,0,10);
  g_pMenuFont->SetLetter('q',189,0,10);
  g_pMenuFont->SetLetter('r',202,0,10);
  g_pMenuFont->SetLetter('s',214,0,10);
  g_pMenuFont->SetLetter('t',226,0,9);
  g_pMenuFont->SetLetter('u',238,0,10);
  g_pMenuFont->SetLetter('v',250,0,10);
  g_pMenuFont->SetLetter('w',263,0,13);
  g_pMenuFont->SetLetter('x',277,0,11);
  g_pMenuFont->SetLetter('y',291,0,10);
  g_pMenuFont->SetLetter('z',304,0,10);
  g_pMenuFont->SetLetter(' ',304,17,5);

  g_pMenuFont->SetLetter('0',0,17,10);
  g_pMenuFont->SetLetter('1',13,17,6);
  g_pMenuFont->SetLetter('2',22,17,10);
  g_pMenuFont->SetLetter('3',34,17,10);
  g_pMenuFont->SetLetter('4',47,17,10);
  g_pMenuFont->SetLetter('5',60,17,10);
  g_pMenuFont->SetLetter('6',73,17,10);
  g_pMenuFont->SetLetter('7',86,17,10);
  g_pMenuFont->SetLetter('8',98,17,10);
  g_pMenuFont->SetLetter('9',111,17,10);
  g_pMenuFont->SetLetter('-',124,17,9);
  g_pMenuFont->SetLetter('=',137,17,9);
  g_pMenuFont->SetLetter('+',146,17,10);
  g_pMenuFont->SetLetter(':',158,17,5);
  g_pMenuFont->SetLetter('/',165,17,9);
  g_pMenuFont->SetLetter('?',177,17,10);
  g_pMenuFont->SetLetter('!',189,17,5);
  g_pMenuFont->SetLetter('@',196,17,13);
  g_pMenuFont->SetLetter('#',211,17,12);
  g_pMenuFont->SetLetter('%',224,17,16);
  g_pMenuFont->SetLetter('*',241,17,11);
  g_pMenuFont->SetLetter('(',254,17,6);
  g_pMenuFont->SetLetter(')',263,17,6);
  g_pMenuFont->SetLetter('&',271,17,11);
  g_pMenuFont->SetLetter(',',283,17,5);
  g_pMenuFont->SetLetter('.',289,17,5);
  g_pMenuFont->SetLetter('[',295,17,6);
  g_pMenuFont->SetLetter(']',302,17,6);
  g_pMenuFont->SetLetter(' ',308,17,6);
  g_pMenuFont->SetLetter('\'',317,17,4);
  
  
  //load main font --------------------------------------------------------------------------------------------------------
  g_pMainFont = new CGFont();
  if (g_pMainFont->LoadFontFromFile(TexData, width, height, "images/mainfont.bmp"))
    return -1;

  if (!g_bForceGDI) {
    SAFE_RELEASE(pMainFontTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pMainFontTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pMainFontTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);
          if (TexData[j]==255) {				
            bytePointer[index]=218; //B
            bytePointer[index+1]=216; //G
            bytePointer[index+2]=208; //R
          } else {
            bytePointer[index]=TexData[j]; //B
            bytePointer[index+1]=TexData[j+1]; //G
            bytePointer[index+2]=TexData[j+2]; //R
          }
          bytePointer[index+3]=TexData[j+3]; //A
          j+=4;
        } 
      }

    pMainFontTex->UnlockRect(0);
    }

  }//if (!g_bForceGDI) 
  SAFE_FREE(TexData);

  g_pMainFont->m_CharHeight = 15;
  g_pMainFont->SetLetter('a',0,0,17);
  g_pMainFont->SetLetter('A',0,0,17);
  g_pMainFont->SetLetter('b',21,0,18);
  g_pMainFont->SetLetter('B',21,0,18);
  g_pMainFont->SetLetter('c',41,0,18);
  g_pMainFont->SetLetter('C',41,0,18);
  g_pMainFont->SetLetter('d',62,0,18);
  g_pMainFont->SetLetter('D',62,0,18);
  g_pMainFont->SetLetter('e',82,0,18);
  g_pMainFont->SetLetter('E',82,0,18);
  g_pMainFont->SetLetter('f',103,0,18);
  g_pMainFont->SetLetter('F',103,0,18);
  g_pMainFont->SetLetter('g',124,0,18);
  g_pMainFont->SetLetter('G',124,0,18);
  g_pMainFont->SetLetter('h',144,0,18);
  g_pMainFont->SetLetter('H',144,0,18);
  g_pMainFont->SetLetter('i',165,0,9);
  g_pMainFont->SetLetter('I',165,0,9);
  g_pMainFont->SetLetter('j',176,0,15);
  g_pMainFont->SetLetter('J',176,0,15);
  g_pMainFont->SetLetter('k',194,0,18);
  g_pMainFont->SetLetter('K',194,0,18);
  g_pMainFont->SetLetter('l',215,0,18);
  g_pMainFont->SetLetter('L',215,0,18);
  g_pMainFont->SetLetter('m',235,0,24);
  g_pMainFont->SetLetter('M',235,0,24);
  g_pMainFont->SetLetter('n',261,0,18);
  g_pMainFont->SetLetter('N',261,0,18);
  g_pMainFont->SetLetter('o',282,0,18);
  g_pMainFont->SetLetter('O',282,0,18);
  g_pMainFont->SetLetter('p',302,0,18);
  g_pMainFont->SetLetter('P',302,0,18);
  g_pMainFont->SetLetter('q',323,0,18);
  g_pMainFont->SetLetter('Q',323,0,18);
  g_pMainFont->SetLetter('r',343,0,18);
  g_pMainFont->SetLetter('R',343,0,18);
  g_pMainFont->SetLetter('s',364,0,18);
  g_pMainFont->SetLetter('S',364,0,18);
  g_pMainFont->SetLetter('t',385,0,18);
  g_pMainFont->SetLetter('T',385,0,18);
  g_pMainFont->SetLetter('u',406,0,18);
  g_pMainFont->SetLetter('U',406,0,18);
  g_pMainFont->SetLetter('v',426,0,18);
  g_pMainFont->SetLetter('V',426,0,18);
  g_pMainFont->SetLetter('w',447,0,24);
  g_pMainFont->SetLetter('W',447,0,24);
  g_pMainFont->SetLetter('x',474,0,18);
  g_pMainFont->SetLetter('X',474,0,18);
  g_pMainFont->SetLetter('y',493,0,18);
  g_pMainFont->SetLetter('Y',493,0,18);
  g_pMainFont->SetLetter('z',515,0,18);
  g_pMainFont->SetLetter('Z',515,0,18);

  g_pMainFont->SetLetter('0',0,18,18);
  g_pMainFont->SetLetter('1',21,18,9);
  g_pMainFont->SetLetter('2',32,18,16);
  g_pMainFont->SetLetter('3',51,18,16);
  g_pMainFont->SetLetter('4',69,18,16);
  g_pMainFont->SetLetter('5',88,18,16);
  g_pMainFont->SetLetter('6',107,18,16);
  g_pMainFont->SetLetter('7',125,18,15);
  g_pMainFont->SetLetter('8',143,18,16);
  g_pMainFont->SetLetter('9',161,18,16);

  g_pMainFont->SetLetter('-',180,18,15);
  g_pMainFont->SetLetter('=',198,18,13);
  g_pMainFont->SetLetter('[',213,17,12);
  g_pMainFont->SetLetter(']',228,17,12);
  g_pMainFont->SetLetter(':',242,17,9);
  g_pMainFont->SetLetter('"',254,14,15);
  g_pMainFont->SetLetter('(',271,17,10);
  g_pMainFont->SetLetter(')',283,17,10);
  g_pMainFont->SetLetter('/',297,17,15);
  g_pMainFont->SetLetter('.',313,17,11);
  g_pMainFont->SetLetter(',',327,17,10);

  g_pMainFont->SetLetter(' ',335,17,18);

  g_pMainFont->SetLetter('#',355,17,13);
  g_pMainFont->SetLetter('!',369,17,19);
  
  /*
  //init fullscreen vertex buffer (currently used for font) ---------------------------------------------------------------
  D3DVERTEXORTHO aFullscreen[4];

  aFullscreen[0].x = 0.f;
  aFullscreen[0].y = 0.f;
  aFullscreen[0].z = 0.5f;
  aFullscreen[0].rhw = 1.0f;
  aFullscreen[0].u = 0.0f;
  aFullscreen[0].v = 0.0f;
  
  aFullscreen[1].x = (float)D3DWindowW;
  aFullscreen[1].y = 0.f;
  aFullscreen[1].z = 0.5f;
  aFullscreen[1].rhw = 1.0f;
  aFullscreen[1].u = 1.0f;
  aFullscreen[1].v = 0.0f;

  aFullscreen[2].x = (float)D3DWindowW;
  aFullscreen[2].y = (float)D3DWindowH;
  aFullscreen[2].z = 0.5f;
  aFullscreen[2].rhw = 1.0f;
  aFullscreen[2].u = 1.0f;
  aFullscreen[2].v = 1.0f;

  aFullscreen[3].x = 0.f;
  aFullscreen[3].y = (float)D3DWindowH;
  aFullscreen[3].z = 0.5f;
  aFullscreen[3].rhw = 1.0f;
  aFullscreen[3].u = 0.0f;
  aFullscreen[3].v = 1.0f;

  SAFE_RELEASE(pFullscreenVB);
  g_pDirect3DDevice->CreateVertexBuffer(sizeof(aFullscreen),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEXORTHO,D3DPOOL_MANAGED,&pFullscreenVB,NULL);

  pFullscreenVB->Lock(0,sizeof(pData),(void**)&pData,0);
  memcpy(pData,aFullscreen,sizeof(aFullscreen));
  pFullscreenVB->Unlock();
*/
  //init controls vertex buffer --------------------------------------------------------------------------------------------
  int texwidth=130;
  if (!g_bForceGDI) {
    D3DVERTEXORTHO aControls[36];
    /*
    for (int i=0; i<6; i++) {
      aControls[i*6+0].x = 380.f+(i*texwidth/6);
      aControls[i*6+0].y = 14.f;
      aControls[i*6+0].z = 0.5f;
      aControls[i*6+0].rhw = 1.0f;
      aControls[i*6+0].u = 0.0f+((float)i/6.f);
      aControls[i*6+0].v = 0.0f;
      
      aControls[i*6+1].x = 380.f+((i+1)*texwidth/6);
      aControls[i*6+1].y = 14.f;
      aControls[i*6+1].z = 0.5f;
      aControls[i*6+1].rhw = 1.0f;
      aControls[i*6+1].u = 0.0f+((float)(i+1)/6.f);
      aControls[i*6+1].v = 0.0f;

      aControls[i*6+2].x = 380.f+((i+1)*texwidth/6);
      aControls[i*6+2].y = 14.f + 9.f;
      aControls[i*6+2].z = 0.5f;
      aControls[i*6+2].rhw = 1.0f;
      aControls[i*6+2].u = 0.0f+((float)(i+1)/6.f);
      aControls[i*6+2].v = 0.5f;

      aControls[i*6+3].x = 380.f+(i*texwidth/6);
      aControls[i*6+3].y = 14.f;
      aControls[i*6+3].z = 0.5f;
      aControls[i*6+3].rhw = 1.0f;
      aControls[i*6+3].u = 0.0f+((float)i/6.f);
      aControls[i*6+3].v = 0.0f;

      aControls[i*6+4].x = 380.f+((i+1)*texwidth/6);
      aControls[i*6+4].y = 14.f + 9.f;
      aControls[i*6+4].z = 0.5f;
      aControls[i*6+4].rhw = 1.0f;
      aControls[i*6+4].u = 0.0f+((float)(i+1)/6.f);
      aControls[i*6+4].v = 0.5f;

      aControls[i*6+5].x = 380.f+(i*texwidth/6);
      aControls[i*6+5].y = 14.f + 9.f;
      aControls[i*6+5].z = 0.5f;
      aControls[i*6+5].rhw = 1.0f;
      aControls[i*6+5].u = 0.0f+((float)i/6.f);
      aControls[i*6+5].v = 0.5f;
    }
    */
    SAFE_RELEASE(pControlsVB);
    g_pDirect3DDevice->CreateVertexBuffer(sizeof(aControls),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEXORTHO,D3DPOOL_MANAGED,&pControlsVB,NULL);
    /*
    pControlsVB->Lock(0,sizeof(pData),(void**)&pData,0);
    memcpy(pData,aControls,sizeof(aControls));
    pControlsVB->Unlock();
    */
  }//if (!g_bForceGDI)

  //loading close/minimize buttons---------------------------------------------------------------------------------------------
  if (TgaLoadTGAToTexture(TexData,width,height,"images/minmax.tga")) {
    return -1;
  }

  if (g_hMinimizeCloseBitmap!=NULL) {
    DeleteObject(g_hMinimizeCloseBitmap);
    g_hMinimizeCloseBitmap=NULL;
  }
  if (g_hMinimizeCloseDC!=NULL) {
    DeleteDC(g_hMinimizeCloseDC);
    g_hMinimizeCloseDC=NULL;
  }
  g_hMinimizeCloseDC = CreateCompatibleDC(GetDC(NULL));
  g_hMinimizeCloseBitmap = CreateBitmap(width,height,1,32,TexData);
  SelectObject(g_hMinimizeCloseDC,g_hMinimizeCloseBitmap);

  SAFE_FREE(TexData);
  
  //loading volume slider ---------------------------------------------------------------------------------------------
  if (TgaLoadTGAToTexture(TexData,width,height,"images/volume_flipped.tga")) {
    return -1;
  }

  if (g_hVolumeBitmap!=NULL) {
    DeleteObject(g_hVolumeBitmap);
    g_hVolumeBitmap=NULL;
  }
  if (g_hVolumeDC!=NULL) {
    DeleteDC(g_hVolumeDC);
    g_hVolumeDC=NULL;
  }
  g_hVolumeDC = CreateCompatibleDC(GetDC(NULL));
  g_hVolumeBitmap = CreateBitmap(width,height,1,32,TexData);
  SelectObject(g_hVolumeDC,g_hVolumeBitmap);

  SAFE_FREE(TexData);

  //loading controls---------------------------------------------------------------------------------------------
  if (TgaLoadTGAToTexture(TexData,width,height,"images/controls.tga")) {
    return -1;
  }
  
  if (!g_bForceGDI) {
    SAFE_RELEASE(pControlsTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pControlsTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pControlsTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);		
          bytePointer[index]=TexData[j]; //B
          bytePointer[index+1]=TexData[j+1]; //G
          bytePointer[index+2]=TexData[j+2]; //R
          
          if (TexData[j+2]==255)
            bytePointer[index+3]=0;
          else
            bytePointer[index+3]=TexData[j+3]; //A

          j+=4;
        } 
      }

    pControlsTex->UnlockRect(0);
    }

  }//if (!g_bForceGDI)
  else {
    if (g_hControlsBitmap!=NULL) {
      DeleteObject(g_hControlsBitmap);
      g_hControlsBitmap=NULL;
    }
    if (g_hControlsDC!=NULL) {
      DeleteDC(g_hControlsDC);
      g_hControlsDC=NULL;
    }
    g_hControlsDC = CreateCompatibleDC(GetDC(NULL));
    g_hControlsBitmap = CreateBitmap(width,height,1,32,TexData);
    SelectObject(g_hControlsDC,g_hControlsBitmap);
  }
  SAFE_FREE(TexData);

  //repeat button---------------------------------------------------------------------------------------------
  if (TgaLoadTGAToTexture(TexData,width,height,"images/repeat.tga")) {
    return -1;
  }
  
  if (!g_bForceGDI) {
    SAFE_RELEASE(pRepeatTex);
    g_pDirect3DDevice->CreateTexture(width,height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pRepeatTex,NULL);
    {
    int j=0;
    D3DLOCKED_RECT locked;
    HRESULT hr=pRepeatTex->LockRect(0,&locked,NULL,D3DLOCK_DISCARD);
      BYTE *bytePointer=(BYTE*)locked.pBits;

      for (DWORD y=0;y<(DWORD)height;y++) {
        DWORD pitch = y*(locked.Pitch);
        for (DWORD x=0;x<(DWORD)width;x++) {
          DWORD index=(x*4+pitch);		
          bytePointer[index]=TexData[j]; //B
          bytePointer[index+1]=TexData[j+1]; //G
          bytePointer[index+2]=TexData[j+2]; //R
          
          if (TexData[j+2]==255)
            bytePointer[index+3]=0;
          else
            bytePointer[index+3]=TexData[j+3]; //A

          j+=4;
        } 
      }

    pRepeatTex->UnlockRect(0);
    }

  }//if (!g_bForceGDI)
  else {
    if (g_hRepeatBitmap!=NULL) {
      DeleteObject(g_hRepeatBitmap);
      g_hRepeatBitmap=NULL;
    }
    if (g_hRepeatDC!=NULL) {
      DeleteDC(g_hRepeatDC);
      g_hRepeatDC=NULL;
    }
    g_hRepeatDC = CreateCompatibleDC(GetDC(NULL));
    g_hRepeatBitmap = CreateBitmap(width,height,1,32,TexData);
    SelectObject(g_hRepeatDC,g_hRepeatBitmap);
  }
  SAFE_FREE(TexData);

  //creating scenes---------------------------------------------------------------------------------------------

  if (!g_bForceGDI) {
    SAFE_RELEASE(g_pCubeVB);
    g_pDirect3DDevice->CreateVertexBuffer(sizeof(g_aCubeVertices),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBA,D3DPOOL_MANAGED,&g_pCubeVB,NULL);
    SAFE_RELEASE(g_pCubeIB);
    g_pDirect3DDevice->CreateIndexBuffer(sizeof(g_aCubeIndices),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&g_pCubeIB,NULL);

    for (int i=0; i<g_Scenes.Length(); i++)
      delete(g_Scenes[i]);

    g_Scenes.DeleteAllElements();

    pData=NULL;
    g_pCubeVB->Lock(0,sizeof(pData),(void**)&pData,0); //lock buffer
    memcpy(pData,g_aCubeVertices,sizeof(g_aCubeVertices)); //copy data
    g_pCubeVB->Unlock();                               //unlock buffer

    g_pCubeIB->Lock(0,sizeof(pData),(void**)&pData,0); //lock buffer
    memcpy(pData,g_aCubeIndices,sizeof(g_aCubeIndices));   //copy data
    g_pCubeIB->Unlock();                               //unlock buffer

    *(g_Scenes.AddElements()) = new CGFxCubeTunnel(g_pDirect3DDevice);
    ((CGFxCubeTunnel*)g_Scenes[0])->m_pCubeVB=g_pCubeVB;
    ((CGFxCubeTunnel*)g_Scenes[0])->m_pCubeIB=g_pCubeIB;

    *(g_Scenes.AddElements()) = new CGFxSpinCube(g_pDirect3DDevice);
    ((CGFxSpinCube*)g_Scenes[1])->m_pCubeVB=g_pCubeVB;
    ((CGFxSpinCube*)g_Scenes[1])->m_pCubeIB=g_pCubeIB;

    *(g_Scenes.AddElements()) = new CGFxStars(g_pDirect3DDevice);
    *(g_Scenes.AddElements()) = new CGFxSphere(g_pDirect3DDevice);
    *(g_Scenes.AddElements()) = new CGFxSpider(g_pDirect3DDevice);
    *(g_Scenes.AddElements()) = new CGFxSpinCubes(g_pDirect3DDevice);
  }//if (!g_bForceGDI)

  return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_ACTIVATE:
      if (g_hD3DWnd!=NULL) SetWindowPos(g_hD3DWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
      g_bIsMinimized=false;
      break;
    case WM_KEYUP:
      //if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;
      switch (wParam) {
        case 33:
          //PAGE UP
          g_iCurrentTrack-=5;
          if (g_iCurrentTrack<0) g_iCurrentTrack = (g_pPlaylist.Length())+g_iCurrentTrack;
          g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
          
          if (g_iCurrentTrack>=g_pPlaylist.Length()+4)
            g_fTargetY=100.f;
          else					
            g_fTargetY-=50.f;

          if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;
          break;		
        case 34:
          //PAGE DOWN
          g_iCurrentTrack+=5;
          if (g_iCurrentTrack>=g_pPlaylist.Length()) g_iCurrentTrack = g_iCurrentTrack-g_pPlaylist.Length();
          g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
          
          if (g_iCurrentTrack<=4)
            g_fTargetY=-100.f;
          else					
            g_fTargetY+=50.f;

          if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;
          break;
        case 35:
          //END
          break;
        case 36:
          //HOME
          break;
        case 40:
          //DOWN
        case 66:
          //B - Next Track
          g_iCurrentTrack++;
          if (g_iCurrentTrack>=g_pPlaylist.Length()) g_iCurrentTrack = 0;
          g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
          
          if (g_iCurrentTrack==0)
            g_fTargetY=-100.f;
          else					
            g_fTargetY+=12.f;

          if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;
          break;
        case 67:
          //C - Pause
          g_pBassLib->PlaybackPause();
          break;
        case 82:
          //R - Repeat
          g_bRepeat = !g_bRepeat;
          break;
        case 86:
          //V
          g_pBassLib->PlaybackStop();
          break;
        case 88:
          //X - Play
          g_pBassLib->PlaybackPlay();
          break;
        case 38:
          //UP
        case 89:
          //Y - Prev Track
          g_iCurrentTrack--;
          if (g_iCurrentTrack<0) g_iCurrentTrack = g_pPlaylist.Length()-1;
          g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
          
          if (g_iCurrentTrack==g_pPlaylist.Length()-1)
            g_fTargetY=100.f;
          else					
            g_fTargetY-=12.f;
          
          if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;
          break;
        case VK_ESCAPE:	{
          PostQuitMessage(0);
          break;
        }
      }
      break;
    case WM_ERASEBKGND:
      return TRUE;
    case WM_LBUTTONDOWN:{
      int x,y;
      x=LOWORD(lParam);
      y=HIWORD(lParam);

      if (x >= 474 && x <= 474+10 &&
          y >= 105 && y <= 105+9) {
            SendMessage(g_hWnd,WM_KEYUP,'R',lParam);
            break;
      }

      if (x >= 494 && x <= 494+70 &&
          y >= 105 && y <= 105+9) {
            int control = (x - 494)/14;
            switch (control){
              case 0:
                SendMessage(g_hWnd,WM_KEYUP,'Y',lParam);
                break;
              case 1:
                SendMessage(g_hWnd,WM_KEYUP,'V',lParam);
                break;
              case 2:
                SendMessage(g_hWnd,WM_KEYUP,'C',lParam);
                break;
              case 3:
                SendMessage(g_hWnd,WM_KEYUP,'X',lParam);
                break;
              case 4:
                SendMessage(g_hWnd,WM_KEYUP,'B',lParam);
                break;
            }
          break;
      }

      if (y > 131 && y < 306) {
        if (g_bForceGDI) {
          y+=D3DWindowY;
        }
        bool hit=false;
        for (int i=0; i<g_pPlaylist.Length(); i++) {
          int x1 = (int)g_pPlaylist[i].xpos+D3DWindowX, x2 = (int)g_pPlaylist[i].xpos + g_pMenuFont->GetStrSize(g_pPlaylist[i].Filename) + D3DWindowX;
          int y1 = (int)g_pPlaylist[i].ypos, y2 = (int)g_pPlaylist[i].ypos + 19;
          if (x >= x1 && x <= x2 &&
              y >= y1+D3DWindowY && y <= y2+D3DWindowY) {
              CGString title = g_pPlaylist[i].Title;
              
              if (i<g_iCurrentTrack)
                g_fTargetY=(i-g_iCurrentTrack)*12.f;
              else
                g_fTargetY=(g_iCurrentTrack-i)*-12.f;

              g_iCurrentTrack = i;
              g_pBassLib->PlayFile(g_pPlaylist[g_iCurrentTrack].Filename);
              if (!g_bScenePlayin&&!g_bNextScene) g_bNextScene=true;

              hit=true;
          }
        }
        if (hit) break;
        if (g_bForceGDI) {
          y-=D3DWindowY;
        }
      }

      if (x >= 562 && x <= 562+18 &&
          y >= 27  && y <= 27+28) {
        SendMessage(g_hWnd,WM_MOUSEMOVE,0,0);
        ShowWindow(g_hWnd,SW_MINIMIZE);
        g_bIsMinimized=true;
        break;
      }

      if (x > 580 && x <= 580+18 &&
          y >= 27  && y <= 27+28) {
        DestroyWindow(g_hWnd);
        break;
      }

      //volume slider requires mousemove message
      if (x > 215 && x < 214+209 &&
            y > 421 && y < 421+24) {
              PostMessage(g_hWnd,WM_MOUSEMOVE,MK_LBUTTON, lParam);
              break;
      }

      SendMessage(g_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
      if (g_hD3DWnd!=NULL) SetWindowPos(g_hD3DWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

      break;
    }
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_MOUSEMOVE:{
      int x,y;
      x=LOWORD(lParam);
      y=HIWORD(lParam);

      for (int i=0; i<6; i++)
        g_bControlHoverstate[i]=false;

      if (x >= 494 && x <= 494+70 &&
          y >= 105 && y <= 105+9) {
            int control = (x - 495)/14;
            g_bControlHoverstate[control] = true;
      }
      if (x >= 474 && x <= 474+10 &&
          y >= 105 && y <= 105+9) {
            g_bControlHoverstate[5] = true;
      }

      bool change=false;

      if (x >= 562 && x <= 562+18 &&
          y >= 27  && y <= 27+28) {
        //hover minimize
            if (!g_bMinimizeHoverstate) {
              if (!g_bForceGDI) BitBlt(g_hBackgroundDC,562,28,18,28,g_hMinimizeCloseDC,0,28,SRCCOPY);
              g_bMinimizeHoverstate=true;
              change=true;
            }
      } else {
        //unhover minimze
        if (g_bMinimizeHoverstate) {
          if (!g_bForceGDI) BitBlt(g_hBackgroundDC,562,28,18,28,g_hMinimizeCloseDC,0,0,SRCCOPY);
          g_bMinimizeHoverstate=false;
          change=true;
        }
      }
      
      if (x > 580 && x <= 580+18 &&
          y >= 27  && y <= 27+28) {
        //hover close
            if (!g_bCloseHoverstate) {
              if (!g_bForceGDI) BitBlt(g_hBackgroundDC,580,28,21,28,g_hMinimizeCloseDC,18,28,SRCCOPY);
              g_bCloseHoverstate=true;
              change=true;
            }
      } else {
        //unhover close
        if (g_bCloseHoverstate) {
          if (!g_bForceGDI) BitBlt(g_hBackgroundDC,580,28,21,28,g_hMinimizeCloseDC,18,0,SRCCOPY);
          g_bCloseHoverstate=false;
          change=true;
        }
      }

      g_fScroller2SpeedOffset=0.f;
      if (y > 300 && y < 320 &&
          x > D3DWindowX && x < D3DWindowW + D3DWindowX) {
        g_fScroller2SpeedOffset=((float)x-320.f)/-640.f;
      }

      if (wParam&MK_LBUTTON) {
        if (x > 215 && x < 214+209 &&
            y > 421 && y < 421+24) {
              
          if (!g_bForceGDI) {
            BitBlt(g_hBackgroundDC,214,421,209,24,g_hVolumeDC,0,0,SRCCOPY);
            BitBlt(g_hBackgroundDC,214,421,(x-215),24,g_hVolumeDC,0,25,SRCCOPY);
          }
          g_pBassLib->SetVolume((int)((float)(x-215)/2.09f));

          change=true;
        }
      }
      
      if (change) {
        SendMessage(g_hWnd,WM_SELFUPDATE,wParam,lParam);
      }
      
      break;
    }
    case WM_SELFUPDATE:{
      HDC hdcScreen=NULL;
      HDC hdcMemory=NULL;

      hdcScreen = GetDC(NULL);
      hdcMemory = CreateCompatibleDC(hdcScreen);

      if (g_bForceGDI) {
        BitBlt(g_hBackgroundDC,0,0,640,480,g_hBackgroundOrigDC,0,0,SRCCOPY);

        //title
        CGString title;
        title = g_pPlaylist[g_iCurrentTrack].Artist;
        title.Add(" - ").Add(g_pPlaylist[g_iCurrentTrack].Title);
        g_pMenuFont->DrawTextGDI(title.Lower(),title.Length(),g_hFontDC,g_hBackgroundDC,120,101);

        //Volume
        //BitBlt(g_hBackgroundDC,214,421,209,24,g_hVolumeDC,0,0,SRCCOPY);
        int volx=(int)((float)g_pBassLib->GetVolume()*2.09f);
        BitBlt(g_hBackgroundDC,214,421,volx,24,g_hVolumeDC,0,25,SRCCOPY);

        //minclose
        if (g_bMinimizeHoverstate) BitBlt(g_hBackgroundDC,562,28,18,28,g_hMinimizeCloseDC,0,28,SRCCOPY);
        if (!g_bMinimizeHoverstate) BitBlt(g_hBackgroundDC,562,28,18,28,g_hMinimizeCloseDC,0,0,SRCCOPY);
        if (g_bCloseHoverstate) BitBlt(g_hBackgroundDC,580,28,21,28,g_hMinimizeCloseDC,18,28,SRCCOPY);			
        if (!g_bCloseHoverstate) BitBlt(g_hBackgroundDC,580,28,21,28,g_hMinimizeCloseDC,18,0,SRCCOPY);

        //playlist
        float yoff=0.f, xoff=0.f;
        for (int i=0; i<g_pPlaylist.Length(); i++) {
          title = g_pPlaylist[i].Artist.Lower();
          title.Add(" - ").Add(g_pPlaylist[i].Title.Lower());

          xoff = D3DWindowX + 250.f + sin((float)(i-g_iCurrentTrack)/(float)g_pPlaylist.Length()*PI*3)*10.f + (sin((float)GetTickCount()/1000.f+i)*5.f);
          yoff = D3DWindowY + 101.f - (float)g_iCurrentTrack * 16.f + g_fTargetY;		

          if (i == g_iCurrentTrack) {
            xoff += sin((float)GetTickCount()/500.f)*8.f;
            g_pMenuFont->DrawTextGDI(title,title.Length(),g_hFontDC,g_hBackgroundDC,xoff,yoff + (float)i * 16.f);
            g_pPlaylist[i].xpos = xoff;
            g_pPlaylist[i].ypos = yoff + (float)i * 16.f;
          } else {
            float ypos=(yoff + (float)i * 16.f);

            if (ypos > D3DWindowY+30 && ypos < D3DWindowY+D3DWindowH-36) g_pMenuFont->DrawTextGDI(title,title.Length(),g_hFontDC,g_hBackgroundDC,xoff,ypos);

            g_pPlaylist[i].xpos = xoff;
            g_pPlaylist[i].ypos = ypos;
          }
        }

        //controls
        BitBlt(g_hBackgroundDC,D3DWindowX+430,D3DWindowY+14,93,9,g_hControlsDC,0,9,SRCAND);
        if (g_bRepeat)
          BitBlt(g_hBackgroundDC,D3DWindowX+410,D3DWindowY+14,10,9,g_hRepeatDC,10,9,SRCAND);
        else
          BitBlt(g_hBackgroundDC,D3DWindowX+410,D3DWindowY+14,10,9,g_hRepeatDC,0,9,SRCAND);


        //bottom scroller
        g_pMenuFont->DrawTextGDI(g_pScrollerText2,g_lScrollerLength,g_hFontDC,g_hBackgroundDC,600.f+g_fBottomScrollerOffset,220.f+D3DWindowY);
        
      }
      HBITMAP oldbmp = (HBITMAP)SelectObject(hdcMemory,g_hBackgroundBitmap);

      RECT windowrect;
      GetWindowRect(g_hWnd,&windowrect);
      POINT windowpos;
      windowpos.x = windowrect.left;
      windowpos.y = windowrect.top;
    
      SIZE windowsize;
      windowsize.cx = windowrect.right - windowrect.left;
      windowsize.cy = windowrect.bottom - windowrect.top;

      POINT destpos={0,0};

      BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

      BOOL bRet=UpdateLayeredWindow(g_hWnd,hdcScreen,&windowpos,&windowsize,g_hBackgroundDC,&destpos,0,&blendPixelFunction,ULW_ALPHA);
      if (bRet==FALSE) {
        DWORD error=GetLastError();
        return -1;
      }
      
      SelectObject(hdcMemory,oldbmp);
      DeleteDC(hdcMemory);
      ReleaseDC(g_hWnd,hdcScreen);
      break;
    }
    case WM_MOVE: {
      int x = (int)(short) LOWORD(lParam);   // horizontal position 
      int y = (int)(short) HIWORD(lParam);   // vertical position 
      if (g_hD3DWnd!=NULL&&hWnd!=g_hD3DWnd) SetWindowPos(g_hD3DWnd,HWND_TOP,x+D3DWindowX,y+D3DWindowY,D3DWindowW,D3DWindowH,SWP_NOACTIVATE);
      break;
                  }
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      
  }
  return 0;
}

long TgaLoadTGAToTexture(unsigned char*& output, int& width, int& height, const char *filename) {
  long i,n,icounter=0;
  unsigned char *ptr,rle,cr,cg,cb,ca=0;
  unsigned char tgaheader[18];
  FILE *stream;

  stream=_fsopen(filename,"rb", 0x20);
  if (stream==NULL)
    return(-1);
  if (fread(tgaheader,1,18,stream)!=18) {
    fclose(stream);
    return(-3);
  }
  if (tgaheader[0]!=0 || tgaheader[1]!=0 || tgaheader[3]!=0 || tgaheader[4]!=0 || (tgaheader[16]!=24 && tgaheader[16]!=32)) {
    fclose(stream);
    return(-4);
  }

  width =tgaheader[12]+(((long)tgaheader[13])<<8);
  height=tgaheader[14]+(((long)tgaheader[15])<<8);
  output = (unsigned char*)malloc(width*height*(tgaheader[16]/8));

  if (tgaheader[2]!=2) {
    // compressed
    ptr=output;
    do {
      rle=fgetc(stream);
      if (rle & 128) {
        // pixel lesen und wiederholen
        cb=fgetc(stream);
        cg=fgetc(stream);
        cr=fgetc(stream);
        if (tgaheader[16]==32)
          ca=fgetc(stream);
        n=(long)(rle & 127)+1;
        for (i=0;i<n;i++) {
          *ptr=cb;	ptr++;
          *ptr=cg;	ptr++;
          *ptr=cr;	ptr++;
          if (tgaheader[16]==32)
            {*ptr=ca;	ptr++;}
        }
      } else {
        // wiederholt pixel lesen
        n=(long)rle+1;
        for (i=0;i<n;i++) {
          *ptr=fgetc(stream);	ptr++;
          *ptr=fgetc(stream);	ptr++;
          *ptr=fgetc(stream);	ptr++;
          if (tgaheader[16]==32)
            {*ptr=fgetc(stream);	ptr++;}
        }
      }
      icounter+=n;
    }
    while (icounter<(long)width*height);
  } else {
    // uncompressed
    if ((int)fread(output, width*(tgaheader[16])/8,height, stream)!=height) {
      fclose(stream);
      return -1;
    }
  }
  fclose(stream);
  return(0);
}

void MakeSomeLove(float Time) {
  if (g_bIsMinimized) return;
  if (g_pDirect3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
    ReleaseAll();
    g_pDirect3DDevice->Reset(&g_PresentParameters);

    InitScene();
    
    g_bScenePlayin = true;
  }

  int i=0;
  CNEPosition pos;
  CNEMatrix4 mat;

  float fft[1024]; 
  g_pBassLib->GetChannelData(fft);
  float fftval=0.f;

  //get single value
  for (int i=0; i<1024; i++)
    fftval+=fft[i];
  fftval/=1024; //normalize
  fftval*=50.f; //amplify

  /**************************************************************************/
  /*                       Actual Scene                                     */
  /**************************************************************************/

  //draw background
  //g_pDirect3DDevice->SetRenderTarget(0,pBackBuffer);
  g_pDirect3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,0x00ffff00,1.0f,0);
  //D3DCOLOR_XRGB(0,0,0)
  g_pDirect3DDevice->BeginScene();

  //render mask to stencil buffer -----------------------------------------------------------------------------
  //disable depth buffering
  g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  g_pDirect3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

  //use alpha blending with src=zero and dest=one to prevent writes to color buffer
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
  g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

  //set up alpha testing
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAREF,0x7f);
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

  //set up stencil function to increment stencil of pixels that pass alpha testing
  g_pDirect3DDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_INCR);
  g_pDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE,TRUE);
  g_pDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);

  //disable lighting
  g_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //select the mask texture	
  g_pDirect3DDevice->SetTexture(0,pBackgroundMaskTex);

  g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
  g_pDirect3DDevice->SetStreamSource(0,pBackgroundVB,0,sizeof(D3DVERTEXORTHO));
  g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

  //re-enable z-buffering
  g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE,TRUE);
  g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,TRUE);

  //set up stencil states for rendering of first scene
  g_pDirect3DDevice->SetRenderState( D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
  g_pDirect3DDevice->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_EQUAL);

  //re-enable lighting
  g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING,TRUE);

  //disable alpha blending and testing
  g_pDirect3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,FALSE);
  g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,FALSE);

  //clear texture
  g_pDirect3DDevice->SetTexture(0,NULL);

  g_pDirect3DDevice->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_NOTEQUAL);
  
  
  //real scene --------------------------------------------------------------------
  g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  
  g_pDirect3DDevice->SetTexture(0,pBackgroundTex);
  g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
  g_pDirect3DDevice->SetStreamSource(0,pBackgroundVB,0,sizeof(D3DVERTEXORTHO));
  g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

  //draw scene
  if (g_bNextScene) {
    if (!g_Scenes[g_iCurrentScene]->PlayOut(fftval,Time)) {
      g_iCurrentScene++;
      g_bScenePlayin=true;
      if (g_iCurrentScene>=g_Scenes.Length()) g_iCurrentScene=0;
      g_bNextScene=false;
    }
  } else if (g_bScenePlayin) {
    if (!g_Scenes[g_iCurrentScene]->PlayIn(fftval,Time))
      g_bScenePlayin=false;
  } else {
    g_Scenes[g_iCurrentScene]->Render(fftval,Time);
  }
  

  //draw main scroller
  static DWORD scrollertime1=GetTickCount();

  g_pDirect3DDevice->SetTexture(0,pMainFontTex);
  if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped()) fftval = 0.f;

  float xoff=((GetTickCount()-scrollertime1)*-.04f)+fftval*25.f;
  if (xoff<-6300.f) scrollertime1=GetTickCount();
  g_pMainFont->DrawSineText(g_pDirect3DDevice,"Titan and Rebels Chipdisc 1 - Oldschool meets Newschool - We're happy to present you the latest and the greatest of our fine chiptune artists. Rebels and Titan joined in full force to bring you this one piece of artwork. We hope you motherfuckers enjoy.      Credits: Graphics: Alien    Code: Gencha    Music: Various Titan and Rebels arists!      ",
    200.f+xoff,110.f,xoff/10.f,2.f,fftval*10.f);


  //Playlist
  //apply scrolling
  if (g_fTargetY<0.f) {
    //scroll up
    g_fTargetY+=(Time*0.002f)-(g_fTargetY*0.002f*Time);
    if (g_fTargetY>0.f)
      g_fTargetY=0.f;
  } else if (g_fTargetY>0.f) {
    //scroll down
    if (g_fTargetY>0.f)
      g_fTargetY-=(Time*0.002f)+(g_fTargetY*0.002f*Time);
    if (g_fTargetY<0.f)
      g_fTargetY=0.f;
  }	

  g_pDirect3DDevice->SetTexture(0,pMenuFontTex);
  CGString title;
  float yoff=0.f;
  for (int i=0; i<g_pPlaylist.Length(); i++) {
    title = g_pPlaylist[i].Artist.Lower();
    title.Add(" - ").Add(g_pPlaylist[i].Title.Lower());

    xoff = 250.f + sin((float)(i-g_iCurrentTrack)/(float)g_pPlaylist.Length()*PI*3)*10.f + (sin((float)GetTickCount()/1000.f+i)*5.f);
    yoff = 101.f - (float)g_iCurrentTrack * 16.f + g_fTargetY;		

    if (i == g_iCurrentTrack) {
      xoff += sin((float)GetTickCount()/500.f)*8.f;
      g_pMenuFont->DrawText(g_pDirect3DDevice,title,title.Length(),xoff,yoff + (float)i * 16.f,D3DCOLOR_ARGB(255,255,255,255),150-((int)xoff-250));
      g_pPlaylist[i].xpos = xoff;
      g_pPlaylist[i].ypos = yoff + (float)i * 16.f;
    } else {
      float alpha=0.f;
      float ypos=(yoff + (float)i * 16.f);

      if (i<g_iCurrentTrack)
        alpha = ypos-20.f;
      else
        alpha = 255-ypos-40.f;

      alpha -= fabs(g_fTargetY);
      if (alpha < 0) alpha = 0;
      if (alpha > 255) alpha = 255;


      g_pMenuFont->DrawText(g_pDirect3DDevice,title,title.Length(),xoff+1.f,ypos+1.f,D3DCOLOR_ARGB((long)alpha,0,0,0),180-((int)xoff-250));
      g_pMenuFont->DrawText(g_pDirect3DDevice,title,title.Length(),xoff,ypos,D3DCOLOR_ARGB((long)alpha,198,206,208),180-((int)xoff-250));
      g_pPlaylist[i].xpos = xoff;
      g_pPlaylist[i].ypos = ypos;
    }
  }

  //Current title
  title = g_pPlaylist[g_iCurrentTrack].Artist;
  title.Add(" - ").Add(g_pPlaylist[g_iCurrentTrack].Title);
  g_pMenuFont->DrawText(g_pDirect3DDevice,title,title.Length(),18,11,D3DCOLOR_ARGB(255,0,0,0));
  g_pMenuFont->DrawText(g_pDirect3DDevice,title,title.Length(),17,10,D3DCOLOR_ARGB(255,198,206,208));
  
  //Bottom scroller
  static DWORD scrollertime2=GetTickCount();
  static long lScroller2Width = g_pMenuFont->GetStrSize(g_pScrollerText2);
  
  if (g_fScroller2SpeedOffset!=0.f)
    g_fBottomScrollerOffset += (g_fScroller2SpeedOffset*Time);
  else if (!g_pBassLib->GetIsPaused() && !g_pBassLib->GetIsStopped())
    g_fBottomScrollerOffset += -0.05f*Time;

  if (g_fBottomScrollerOffset<-(lScroller2Width+600) || g_fBottomScrollerOffset>0.f) g_fBottomScrollerOffset=0.f;

  g_rBottomScroller = g_pMenuFont->DrawText(g_pDirect3DDevice,g_pScrollerText2,g_lScrollerLength,
    605.f+g_fBottomScrollerOffset+1.f,215.f+1.f,D3DCOLOR_ARGB((long)(50+fftval*255),0,0,0));
  g_pMenuFont->DrawText(g_pDirect3DDevice,g_pScrollerText2,g_lScrollerLength,
    605.f+g_fBottomScrollerOffset,215.f,D3DCOLOR_ARGB((long)(50+fftval*255),228,236,238));
  
  //Draw Controls

  //first update vertex buffer (dirty)
  VOID* pData=NULL;
  int texwidth=70;
  D3DVERTEXORTHO* aControls;

  pControlsVB->Lock(0,0,(void**)&aControls,0);
  for (int i=0; i<5; i++) {
    float x1=430.f+(i*texwidth/5);
    float x2=430.f+((i+1)*texwidth/5);

    aControls[i*6+0].x = x1;
    aControls[i*6+0].y = 14.f;
    aControls[i*6+0].z = 0.5f;
    aControls[i*6+0].rhw = 1.0f;
    aControls[i*6+0].u = i*(1.f/(float)5);
    aControls[i*6+0].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;
    
    aControls[i*6+1].x = x2;
    aControls[i*6+1].y = 14.f;
    aControls[i*6+1].z = 0.5f;
    aControls[i*6+1].rhw = 1.0f;
    aControls[i*6+1].u = (i+1)*(1.f/(float)5);
    aControls[i*6+1].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;

    aControls[i*6+2].x = x2;
    aControls[i*6+2].y = 14.f + 9.f;
    aControls[i*6+2].z = 0.5f;
    aControls[i*6+2].rhw = 1.0f;
    aControls[i*6+2].u = (i+1)*(1.f/(float)5);
    aControls[i*6+2].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;

    aControls[i*6+3].x = x1;
    aControls[i*6+3].y = 14.f;
    aControls[i*6+3].z = 0.5f;
    aControls[i*6+3].rhw = 1.0f;
    aControls[i*6+3].u = i*(1.f/(float)5);
    aControls[i*6+3].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;

    aControls[i*6+4].x = x2;
    aControls[i*6+4].y = 14.f + 9.f;
    aControls[i*6+4].z = 0.5f;
    aControls[i*6+4].rhw = 1.0f;
    aControls[i*6+4].u = (i+1)*(1.f/(float)5);
    aControls[i*6+4].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;

    aControls[i*6+5].x = x1;
    aControls[i*6+5].y = 14.f + 9.f;
    aControls[i*6+5].z = 0.5f;
    aControls[i*6+5].rhw = 1.0f;
    aControls[i*6+5].u = i*(1.f/(float)5);
    aControls[i*6+5].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;
  }

  i=5;
  aControls[i*6+0].x = 410.f;
  aControls[i*6+0].y = 14.f;
  aControls[i*6+0].z = 0.5f;
  aControls[i*6+0].rhw = 1.0f;
  aControls[i*6+0].u = (g_bRepeat)?.5f:0.f;
  aControls[i*6+0].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;
  
  aControls[i*6+1].x = 420.f;
  aControls[i*6+1].y = 14.f;
  aControls[i*6+1].z = 0.5f;
  aControls[i*6+1].rhw = 1.0f;
  aControls[i*6+1].u = (g_bRepeat)?1.f:.5f;
  aControls[i*6+1].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;

  aControls[i*6+2].x = 420.f;
  aControls[i*6+2].y = 14.f + 9.f;
  aControls[i*6+2].z = 0.5f;
  aControls[i*6+2].rhw = 1.0f;
  aControls[i*6+2].u = (g_bRepeat)?1.f:.5f;
  aControls[i*6+2].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;

  aControls[i*6+3].x = 410.f;
  aControls[i*6+3].y = 14.f;
  aControls[i*6+3].z = 0.5f;
  aControls[i*6+3].rhw = 1.0f;
  aControls[i*6+3].u = (g_bRepeat)?.5f:0.f;
  aControls[i*6+3].v = ((g_bControlHoverstate[i])?0.5f:0.0f) + 0.05f;

  aControls[i*6+4].x = 420.f;
  aControls[i*6+4].y = 14.f + 9.f;
  aControls[i*6+4].z = 0.5f;
  aControls[i*6+4].rhw = 1.0f;
  aControls[i*6+4].u = (g_bRepeat)?1.f:.5f;
  aControls[i*6+4].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;

  aControls[i*6+5].x = 410.f;
  aControls[i*6+5].y = 14.f + 9.f;
  aControls[i*6+5].z = 0.5f;
  aControls[i*6+5].rhw = 1.0f;
  aControls[i*6+5].u = (g_bRepeat)?.5f:0.f;
  aControls[i*6+5].v = ((g_bControlHoverstate[i])?1.0f:0.5f) + 0.05f;

  pControlsVB->Unlock();

  g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  
  g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
  g_pDirect3DDevice->SetStreamSource(0,pControlsVB,0,sizeof(D3DVERTEXORTHO));
  
  g_pDirect3DDevice->SetTexture(0,pControlsTex);
  g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,10);
  g_pDirect3DDevice->SetTexture(0,pRepeatTex);
  g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,30,2);

  /*
  //draw analyzer
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
  g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  g_pDirect3DDevice->SetTexture(0,NULL);
  g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

  float barwidth = 42.f;
  float barheight = 4.f;
  float analyzerx = 85.f;
  float analyzery = 240.f;
  D3DVERTEXORTHORGBA vdata[4];

  int b0=0;
  for (int bars=0; bars<7; bars++) {
    float fftbarval=0.f;
    
    for (int i=(1024/7*bars); i<(1024/7*bars+1); i++)
      fftbarval+=fft[i];
  
    fftbarval/=(1024/7); //normalize
    fftbarval*=5000.f; //amplify
    
    barvals[bars]=fftbarval;

    float sum=0;
    int sc,b1=pow(2.f,(float)(bars*10.0/(8-1)));
    if (b1>1023) b1=1023;
    if (b1<=b0) b1=b0+1;
    sc=10+b1-b0;
    for (;b0<b1;b0++) sum+=fft[16+b0];
    float y=(sqrt(sum/log10((float)sc))*1.7*1000)-4;
  
    fftbarval = y/300.f;

    for (int peaks=0; peaks<(int)fftbarval; peaks++) {
      vdata[0].x = analyzerx;
      vdata[0].y = analyzery;
      vdata[0].z = 0.5f;
      vdata[0].rhw = 0.5f;
      vdata[0].rgba = D3DCOLOR_ARGB(80,180,180,180);
      
      vdata[1].x = analyzerx + barwidth; 
      vdata[1].y = analyzery;
      vdata[1].z = 0.5f;
      vdata[1].rhw = 0.5f;
      vdata[1].rgba = D3DCOLOR_ARGB(80,180,180,180);
      
      vdata[2].x = analyzerx + barwidth;
      vdata[2].y = analyzery + barheight;
      vdata[2].z = 0.5f;
      vdata[2].rhw = 0.5f;
      vdata[2].rgba = D3DCOLOR_ARGB(80,180,180,180);
      
      vdata[3].x = analyzerx;
      vdata[3].y = analyzery + barheight;
      vdata[3].z = 0.5f;
      vdata[3].rhw = 0.5f;
      vdata[3].rgba = D3DCOLOR_ARGB(80,180,180,180);

      g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHORGBA);
      g_pDirect3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&vdata,sizeof(D3DVERTEXORTHORGBA));

      analyzery-=(barheight+1.f);
    }

    analyzery = 240.f;
    analyzerx+=barwidth+10;
  }
  */

  g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
  g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
  //flash
  if (fftval>.2f) {
    g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
    g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

    g_pDirect3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(100,0,0,0));
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TFACTOR);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    
    g_pDirect3DDevice->SetTexture(0,NULL);
    g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
    g_pDirect3DDevice->SetStreamSource(0,pBackgroundVB,0,sizeof(D3DVERTEXORTHO));
    g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
  }

  //pause/stop
  if (g_pBassLib->GetIsPaused()||g_pBassLib->GetIsStopped()) {
    g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
    g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

    g_pDirect3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(100,55,0,0));
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TFACTOR);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    
    g_pDirect3DDevice->SetTexture(0,NULL);
    g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
    g_pDirect3DDevice->SetStreamSource(0,pBackgroundVB,0,sizeof(D3DVERTEXORTHO));
    g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
  }
  

  //draw curves back over scene --------------------------------------------------------------------
  g_pDirect3DDevice->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_EQUAL);
  
  g_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
  g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
  g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  g_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  
  g_pDirect3DDevice->SetTexture(0,pBackgroundTex);
  g_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);
  g_pDirect3DDevice->SetStreamSource(0,pBackgroundVB,0,sizeof(D3DVERTEXORTHO));
  g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

  g_pDirect3DDevice->EndScene();
  
  g_pDirect3DDevice->Present(NULL,NULL,NULL,NULL);

  return;
}

void MakeSomeGDILove(float Time) {
  if (g_bIsMinimized) return;

  //apply scrolling
  if (g_fTargetY<0.f) {
    //scroll up
    g_fTargetY+=(Time*0.002f)-(g_fTargetY*0.002f*Time);
    if (g_fTargetY>0.f)
      g_fTargetY=0.f;
  } else if (g_fTargetY>0.f) {
    //scroll down
    if (g_fTargetY>0.f)
      g_fTargetY-=(Time*0.002f)+(g_fTargetY*0.002f*Time);
    if (g_fTargetY<0.f)
      g_fTargetY=0.f;
  }	

  static DWORD scrollertime2=GetTickCount();
  static long lScroller2Width = g_pMenuFont->GetStrSize(g_pScrollerText2);
  
  if (g_fScroller2SpeedOffset!=0.f)
    g_fBottomScrollerOffset += (g_fScroller2SpeedOffset*Time);
  else if (!g_pBassLib->GetIsPaused() && !g_pBassLib->GetIsStopped())
    g_fBottomScrollerOffset += -0.05f*Time;

  if (g_fBottomScrollerOffset<-(lScroller2Width+600) || g_fBottomScrollerOffset>0.f) g_fBottomScrollerOffset=0.f;
  
  PostMessage(g_hWnd,WM_SELFUPDATE,NULL,NULL);
  return;
}