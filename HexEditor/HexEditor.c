#include "HexEditor.h"
#include "Graphics.h"

wchar_t           CFGfile[128];
wchar_t           _file_open[256];

int           offset=0;
int          file_size=0;
UINT8           *buffer = NULL;

OPEN_MODE_T     open_mode; // тип открываемого ресурса

int   f;

int          seem_id=0, record=0;


VIEW_MODES_T    view_mode=HEX_MODE;

UIS_DIALOG_T            dialog;
DRAWING_BUFFER_T		bufd; // буфер для рисования

int          ENTRIES_NUM=0;		// кол-во пунктов
FILEINFO        *file_list=NULL; // файлы
wchar_t           cur_folder[256];  // текущая папка

wchar_t           find_text[256];


/* Обработчики событий для каждого state-а */

/* Обработчики событий для HW_STATE_ANY (используется в любом state) */
static const EVENT_HANDLER_ENTRY_T any_state_handlers[] =
{  
    /* GUI-приложения должы ловить этот ивент */
    { EV_REVOKE_TOKEN,              APP_HandleUITokenRevoked },
    { STATE_HANDLERS_END,           NULL           },
};

static const EVENT_HANDLER_ENTRY_T init_state_handlers[] =
{
    /* Это событие означает, что мы можем работать с UI */
    { EV_GRANT_TOKEN,               HandleUITokenGranted },
    { STATE_HANDLERS_END,           NULL           },
};

static EVENT_HANDLER_ENTRY_T main_state_handlers[] =
{
    //{ EV_DIALOG_DONE,                   destroyApp },
    //{ EV_DONE,                          destroyApp },

    
    { EV_INK_KEY_PRESS,             HandleKeypress },
    { STATE_HANDLERS_END,                     NULL },
};

static const EVENT_HANDLER_ENTRY_T edit_state_handlers[] =
{
    { EV_DATA,                               	 EditData  },
    { EV_DONE,                       		       EditOk  },
	{ EV_CANCEL,                 				     Back  },
    { STATE_HANDLERS_END,                             NULL },
};

static  EVENT_HANDLER_ENTRY_T menu_state_handlers[] =
{
    { EV_DIALOG_DONE,             	            destroyApp },
    { EV_REQUEST_LIST_ITEMS,       	        HandleListReq  },
	{ EV_SELECT, 		                       mSelectItem },

    { EV_DONE,                     	                  Back },
    { STATE_HANDLERS_END,           		      	  NULL },
};

static  EVENT_HANDLER_ENTRY_T fbrowser_state_handlers[] =
{
    { EV_DIALOG_DONE,             	            destroyApp },
    { EV_REQUEST_LIST_ITEMS,       	        HandleListReq  },
	{ EV_SELECT, 		                      fbSelectItem },

    { EV_DONE,                     	                  Back },
    { STATE_HANDLERS_END,           		      	  NULL },
};

static const EVENT_HANDLER_ENTRY_T ram_state_handlers[] =
{
    { EV_DATA,                               	 RAMData  },
    { EV_DONE,                       		       EditOk  },
	{ EV_CANCEL,                 				     Back  },
    { STATE_HANDLERS_END,                             NULL },
};

static const EVENT_HANDLER_ENTRY_T seem_state_handlers[] =
{
    { EV_DATA,                               	 SEEMData  },
    { EV_DONE,                       		       EditOk  },
	{ EV_CANCEL,                 				     Back  },
    { STATE_HANDLERS_END,                             NULL },
};

static const EVENT_HANDLER_ENTRY_T find_state_handlers[] =
{
    { EV_DATA,                               	 FindData  },
    { EV_DONE,                       		       EditOk  },
	{ EV_CANCEL,                 				     Back  },
    { STATE_HANDLERS_END,                             NULL },
};

static const EVENT_HANDLER_ENTRY_T goto_state_handlers[] =
{
    { EV_DATA,                               	 GoToData  },
    { EV_DONE,                       		       EditOk  },
	{ EV_CANCEL,                 				     Back  },
    { STATE_HANDLERS_END,                             NULL },
};

/* Таблица соответствий обработчиков, состояний и функций входа-выхода из состояния.
   Порядок состояний такой же, как в enum-e */
static const STATE_HANDLERS_ENTRY_T state_handling_table[] =
{
    { HE_STATE_ANY,               // State
      NULL,                       // Обработчик входа в state
      NULL,                       // Обработчик выхода из state
      any_state_handlers          // Список обработчиков событий
    },

    { HE_STATE_INIT,
      NULL,
      NULL,
      init_state_handlers
    },
    
    { HE_STATE_MAIN,
      MainStateEnter,
      StateExit,
      main_state_handlers
    },

    { HE_STATE_EDIT,
      EditStateEnter,
      StateExit,
      edit_state_handlers
    },

    { HE_STATE_MENU,
      MenuStateEnter,
      StateExit,
      menu_state_handlers
    },

    { HE_STATE_FBROWSER,
      FbrowserStateEnter,
      FbrowserStateExit,
      fbrowser_state_handlers
    },

    { HE_STATE_RAM,
      RAMStateEnter,
      StateExit,
      ram_state_handlers
    },

    { HE_STATE_SEEM,
      SEEMStateEnter,
      StateExit,
      seem_state_handlers
    },

    { HE_STATE_FIND,
      FindStateEnter,
      StateExit,
      find_state_handlers
    },

    { HE_STATE_GOTO,
      GoToStateEnter,
      StateExit,
      goto_state_handlers
    }
};

/* Будет добавлено в следующую версию elfpack-а */
UINT32 LdrInitEventHandlersTbl( EVENT_HANDLER_ENTRY_T *tbl,  UINT32 *base )
{
	UINT32			i=0;
	while( tbl[i].code!=STATE_HANDLERS_END )
	{
		if(tbl[i].code==STATE_HANDLERS_RESERVED)	
		{
			tbl[i].code = (*base)++;
		}
		i++;
	}
	return *base;
}

UINT32 LdrFindEventHandlerTbl( EVENT_HANDLER_ENTRY_T *tbl,  EVENT_HANDLER_T *hfn )
{
	UINT32			i=0;
	while( tbl[i].code!=STATE_HANDLERS_END )
	{
		if(tbl[i].hfunc==hfn)	
		{
			return tbl[i].code;
		}
		i++;
	}

	return 0;
}

int DL_FsReadFile(char *buffer, int, int size, int file_handle, int *err)
{
  return(w_fread(file_handle, buffer, size));
}

int util_asc2ul( const char * s, int count )
{
    char    b;
    iint    res = 0;
    
    while ( count > 0 ) {
        b = *s++;
        if ( b >= '0' && b <= '9' ) {
            b -= '0';
        } else if ( b >= 'A' && b <= 'F' ) {
            b = b - 'A' + 10;
        } else if ( b >= 'a' && b <= 'f' ) {
            b = b - 'a' + 10;
        } else
            b = 0;
        res = res << 4;
        res |= b;
        count--;
    }
    
    return res;
}


/* Это entry для всех эльфов, должна быть объявлена именно так */
/* file_uri - путь к эльфу (аналогично argv[0]) */
/* param - параметры эльфа (аналогично остальным argv) */
/* reserve - выделенная база eventcode-ов (зарезервировано 64 значения)*/
int Register( wchar_t* file_open,  int reserve )
{
    int status = RESULT_OK;
    /* Здесь мы будем хранить выданную нам "базу" ивентов */
    //UINT32              evcode_base;
	
   // evcode_base = reserve;    // Сохраняем reserve
	// Дозаполнем наши таблицы обработчиков, где есть STATE_HANDLERS_RESERVED
	//LdrInitEventHandlersTbl( main_state_handlers, &evcode_base );

    // читаем ленг
    wstrcpy(_file_open, file_open);
    wstrcpy(CFGfile, file_open);
    CFGfile[wstrlen(file_open)-3] = 0;
    wstrcat(CFGfile, L"lng");	
    ReadLang(CFGfile);

    // читаем конфиг
    CFGfile[wstrlen(file_open)-3] = 0;
    wstrcat(CFGfile, L"cfg");

    
    if(file_open)
    {
        OpenFile(file_open);
        LoadBuffer(0);
    }
    else
    {
        ReadSetting(CFGfile);
        switch (open_mode)
        {
        case OPEN_FILE:
            OpenFile(file_open);
            LoadBuffer(0);
            break;

        case OPEN_RAM:
            OpenRAM(offset);
            break;
        }
    }

        

    

/* Регистрируем приложение */
    //status = APP_Register( &evcode_base,            // На какое событие должно запускаться приложение
    //                       1,                       // Кол-во событий, на которое должно запускаться приложение
    //                       state_handling_table,    // Таблица состояний
    //                       HE_STATE_MAX,            // Количество состояний
    //                       (void*)startApp );   // Функция, вызываемая при запуске
                   
    
    //LdrStartApp(evcode_base);   // Запустить немедленно
    
    return 1;   // Пока не имеет значения, что возвращать
}


/* Функция вызываемая при старте приложения */
int startApp( EVENT_STACK_T *ev_st,  REG_ID_T reg_id,  UINT32 param2 )//void *reg_hdl )
{
    //APP_HEXEDITOR_T     *app = NULL;
    int status = RESULT_OK;


    /* Инициализация для фоновых приложений */
    //app = (APP_HEXEDITOR_T*)APP_InitAppData( (void *)APP_HandleEvent, // Обработчик для приложений с GUI
    //                                          sizeof(APP_HEXEDITOR_T), // Размер структуры приложения
    //                                          reg_id,
    //                                          0, 1,
    //                                          1,
    //                                          1, 1, 0 );

    InitResources();

    //status = APP_Start( ev_st,
    //                    &app->apt,
    //                    HE_STATE_INIT, // Начальное состояние
    //                    state_handling_table,
    //                    destroyApp,
    //                    app_name,
     //                   0 );

    find_text[0] = 0;


    return RESULT_OK;
}


/* Функция выхода из приложения */
UINT32 destroyApp( EVENT_STACK_T *ev_st,  void *app )
{
    int  status;
    //APPLICATION_T           *papp = (APPLICATION_T*) app;

    // освобождаем все ресурсы
    free(file_list);
    if (f != FILE_HANDLE_INVALID) free(buffer);
    w_fclose( f);
    f = FILE_HANDLE_INVALID;
    RemoveResources();
    CloseLang();
    //APP_UtilUISDialogDelete(  &papp->dialog ); // удалем диалог

    /* Завершаем работу приложения */
    //status = APP_Exit( ev_st, app, 0 );

    /* Выгружаем эльф */
    //LdrUnloadELF(&Lib); // &Lib указывает на начало эльфа

    return status;
}



// записываем в конфиг
int SaveSetting(wchar_t *cfg_file)
{
	//int written;
	FILE_HANDLE_T fh;

	// запись в файл
	fh = w_fopen(cfg_file, WA_Create | WA_Write, 0x1FF, 0);
    if (fh == FILE_HANDLE_INVALID) return RESULT_FAIL;

    w_fwrite(fh, &open_mode, 1);
    switch (open_mode)
    {
    case OPEN_FILE:
        //DL_FsWriteFile( file_open, 256*sizeof(wchar_t), 1, fh, &written );
        w_fwrite(fh, _file_open, 256*sizeof(wchar_t));
        break;

    case OPEN_RAM:
        w_fwrite(fh, &offset, sizeof(INT32));
        break;
    }
    
	w_fclose(fh);

	return RESULT_OK;
}


int ReadSetting(wchar_t *cfg_file)
{
	//int r;
	FILE_HANDLE_T fh;

	// запись в файл
	fh = w_fopen(cfg_file, WA_READ, 0x1FF, 0);
    if (fh == FILE_HANDLE_INVALID) return RESULT_FAIL;

    open_mode = 0;
    w_fread(fh, &open_mode, 1);
    
    switch (open_mode)
    {
    case OPEN_FILE:
        _file_open[0] = 0;
        w_fread( fh, _file_open, 256*sizeof(wchar_t));
        break;

    case OPEN_RAM:
        w_fread( fh, &offset, sizeof(int));
        break;
    }
    
	w_fclose(fh);

	return RESULT_OK;
}

/* В этой функции мы будем создавать все необходимые нам ресурсы */
int InitResources( )
{
    int status;

	const wchar_t list_caption[] = L"/";
    const wchar_t edit_caption[] = L"HEX";


	/* Ресурс для заголовка списка */
    status = DRM_CreateResource( &Resources[RES_LIST_CAPTION], RES_TYPE_STRING, (void*)list_caption, (wstrlen((wchar_t*)list_caption)+1)*sizeof(wchar_t) );
	if( status!=RESULT_OK ) return status;

	status |= DRM_CreateResource( &Resources[RES_EDIT_CAPTION], RES_TYPE_STRING, (void*)edit_caption, (wstrlen((wchar_t*)edit_caption)+1)*sizeof(wchar_t) );
    if( status!=RESULT_OK ) return status;
	

	return status;
}

/* В этой функции мы будем освобождать ресурсы */
UINT32 RemoveResources( )
{
	UINT32				status = RESULT_OK;
	UINT32				i;

	for(i=0;i<RES_MAX;i++)
		status |= DRM_ClearResource( Resources[i] );

	return status;
}



/* Обработчик входа в main state */
int MainStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    SU_PORT_T               port = papp->port;

    
	if(type!=ENTER_STATE_ENTER) return RESULT_OK;


		
	dialog = GC_CreateMemoryGC ( Display_GetWidth(0), Display_GetHeight(0), 16, 0, 0, 0 ); // создаём канвас

    if(dialog == 0) return RESULT_FAIL;

    papp->dialog = dialog;

	
	paint();



    return RESULT_OK;
}

UINT32 StateExit( EVENT_STACK_T *ev_st,  void *app,  EXIT_STATE_TYPE_T type )
{
	APPLICATION_T           *papp = (APPLICATION_T*) app;

    if ( type == EXIT_STATE_SUSPEND )
      return RESULT_OK;

	APP_UtilUISDialogDelete( &papp->dialog );

	return RESULT_OK;
}

UINT32 EditStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UIS_DIALOG_T            dialog = NULL;
    wchar_t                   buf[8];
    wchar_t                   buf_offset[64];
    int                  num_chars=2;
    int                   edit_type=1;
   

    if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

    switch(view_mode)
    {
    case HEX_MODE:
        num_chars = 2;
        edit_type = 1;
        wstrcpy(buf_offset, L"HEX, 0x");
        dec2hex(buffer[RelativeOffset(offset)], buf, 2);
        break;

    case DEC_MODE:
        num_chars = 3;
        edit_type = 32; 
        wstrcpy(buf_offset, L"DEC, 0x");
        u_ltou(buffer[RelativeOffset(offset)], buf);
        break;

    case TEXT_MODE:
        num_chars = 1;
        edit_type = 1;
        wstrcpy(buf_offset, L"TEXT, 0x");
        buf[0] = buffer[RelativeOffset(offset)];
        buf[1] = 0;
        break;
    }

    // заголовок 
    dec2hex(offset, buf_offset+wstrlen(buf_offset), 8);
    DRM_SetResource( Resources[RES_EDIT_CAPTION], (void*)buf_offset, (wstrlen(buf_offset)+1)*sizeof(wchar_t));


        dialog = UIS_CreateCharacterEditor( &papp->port,
                                            buf,
                                            edit_type,
                                            num_chars,
                                            FALSE,
                                            NULL,            
                                            Resources[RES_EDIT_CAPTION] );


        if(dialog == NULL) return RESULT_FAIL;

        papp->dialog = dialog;


        return RESULT_OK;
}

UINT32 EditData( EVENT_STACK_T *ev_st,  void *app )
{
    UINT32       status = 0;
    EVENT_T      *event = AFW_GetEv(ev_st);
    char         buf[4];
    UINT8        value;


    switch(view_mode)
    {
    case HEX_MODE:
        u_utoa( event->attachment, buf);
        value = strtoul( buf, NULL, 16 );// строка в  long
        break;

    case DEC_MODE:
        value  = u_atol( event->attachment);
        break;

    case TEXT_MODE:
        UCS2toCP1251(event->attachment, buf);
        value = buf[0];
        break;
    }

    SaveOffset(offset, value);
   
    status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    
    return status;
}

UINT32 EditOk( EVENT_STACK_T *ev_st,  void *app )
{
     UINT32                  status = 0;
     ADD_EVENT_DATA_T        *ev_data;
     APPLICATION_T           *papp = (APPLICATION_T*) app;

     AFW_AddEvEvD(ev_st, EV_REQUEST_DATA, ev_data);
	 UIS_HandleEvent( papp->dialog,  ev_st );

     return status;
}

UINT32 Back( EVENT_STACK_T *ev_st,  void *app )
{
    UINT32                	    status = 0;
    APPLICATION_T               *papp = (APPLICATION_T*)app;

    switch (papp->state) {
    case HE_STATE_MENU:
    case HE_STATE_EDIT:
        status = APP_UtilChangeState(HE_STATE_MAIN, ev_st, app );
        break;

    case HE_STATE_FBROWSER:
    case HE_STATE_RAM:
    case HE_STATE_SEEM:
    case HE_STATE_FIND:
    case HE_STATE_GOTO:
        status = APP_UtilChangeState(HE_STATE_MENU, ev_st, app );
        break;

    }
    
    return status;
}


UINT32 MenuStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APP_HEXEDITOR_T           *papp = (APP_HEXEDITOR_T*)app;
    SU_PORT_T               port = papp->apt.port;
	UINT32					starting_num;


	if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

    DRM_SetResource( Resources[RES_LIST_CAPTION], (void*)L"Menu", (wstrlen(L"Menu")+1)*sizeof(wchar_t));

    	dialog = UIS_CreateList( &port,
    							 0,
    							 MENU_MAX,
    							 0,
    							 &starting_num,
    							 0,
    							 2,
    							 NULL,
    							 Resources[RES_LIST_CAPTION] );
      
    
        if(dialog == NULL) return RESULT_FAIL;
    
        papp->apt.dialog = dialog;

        
    	// Чтобы список заработал, ему нужно сразу передать starting_num элементов
    	mSendListItems(ev_st, app, 1, starting_num);

    return RESULT_OK;
}


/*
UINT32 FbrowserStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APP_HEXEDITOR_T         *papp = (APP_HEXEDITOR_T*)app;
    SU_PORT_T               port = papp->apt.port;
	UINT32					starting_num;
    INT16                   i;



	if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;


    if (file_open[0] == 0) {
         wstrcpy(cur_folder, L"/");
    } else {
        wstrcpy(cur_folder, file_open);
        for (i=wstrlen(file_open)-1; i > 0 && file_open[i] != '/' ; i--);
        cur_folder[i+1] = 0;
    }
    
    FindFile(cur_folder, L"*"); // поиск файлов

    DRM_SetResource( Resources[RES_LIST_CAPTION], (void*)cur_folder, (wstrlen(cur_folder)+1)*sizeof(wchar_t));

    	dialog = UIS_CreateList( &port,
    							 0,
    							 ENTRIES_NUM,
    							 0,
    							 &starting_num,
    							 0,
    							 2,
    							 NULL,
    							 Resources[RES_LIST_CAPTION] );
      
    
        if(dialog == NULL) return RESULT_FAIL;
    
        papp->apt.dialog = dialog;

        
    	// Чтобы список заработал, ему нужно сразу передать starting_num элементов
    	fbSendListItems(ev_st, app, 1, starting_num);

    return RESULT_OK;
}


UINT32 FbrowserStateExit( EVENT_STACK_T *ev_st,  void *app,  EXIT_STATE_TYPE_T type )
{
	APPLICATION_T           *papp = (APPLICATION_T*) app;

    if ( type == EXIT_STATE_SUSPEND )
      return RESULT_OK;

    free(file_list);
	APP_UtilUISDialogDelete( &papp->dialog );

	return RESULT_OK;
}

/* Функция-обработчик события EV_REQUEST_LIST_ITEMS - диалоги типа List с помощью этого ивента
    запрашивают очередную порцию данных от приложения */
UINT32 HandleListReq( EVENT_STACK_T *ev_st,  void *app )
{
	APP_HEXEDITOR_T			*papp = (APP_HEXEDITOR_T*) app;
	EVENT_T					*event;
	UINT32					start, num;

	// Если у нашего приложения нет "фокуса", значит сообщение пришло не для нас
	if( !papp->apt.focused ) return RESULT_OK;

	// Извлекаем из ивента нужные нам данные
	event = AFW_GetEv( ev_st );

	start = event->data.list_items_req.begin_idx;
	num = event->data.list_items_req.count;

	// Мы поймали ивент, который предназначается только нам - значит, мы должны его убрать из списка
	APP_ConsumeEv( ev_st, app );

    switch (papp->apt.state) {
    case HE_STATE_MENU:
        mSendListItems( ev_st, app, start, num );
        break;
     case HE_STATE_FBROWSER:
        fbSendListItems( ev_st, app, start, num );
        break;
    }

    return RESULT_OK;
}

UINT32 mSendListItems( EVENT_STACK_T *ev_st,  void *app, UINT32 start, UINT32 num)
{
    APPLICATION_T			*papp = (APPLICATION_T*) app;
	LIST_ENTRY_T			*plist=NULL; // Буффер для элементов списка
	UINT32					i, index, status=RESULT_OK;
    
    if( num==0 ) return RESULT_FAIL; // Контролируем параметр, на всякий случай
	
	plist = malloc( sizeof(LIST_ENTRY_T)*num ); // malloc
	if( plist==NULL ) return RESULT_FAIL;

    for( index=0, i=start; (i<start+num) && (i<=MENU_MAX+1); i++, index++) {

        plist[index].editable = FALSE;			// Поле нередактируемо
		plist[index].content.static_entry.unk6 = 1;
	}

    UIS_MakeContentFromString( "q0", &(plist[0].content.static_entry.text), RecLang(LNG_OPEN_FILE));
    UIS_MakeContentFromString( "q0", &(plist[1].content.static_entry.text), RecLang(LNG_OPEN_RAM));
    UIS_MakeContentFromString( "q0", &(plist[2].content.static_entry.text), RecLang(LNG_OPEN_SEEM));
    UIS_MakeContentFromString( "q0", &(plist[3].content.static_entry.text), RecLang(LNG_FIND));
    UIS_MakeContentFromString( "q0", &(plist[4].content.static_entry.text), RecLang(LNG_GOTO));



    // Добавляем в текущий список ивентов новый ивент, предназначенный для нашего списка
	status = APP_UtilAddEvUISListData( ev_st, app, 0,
									   start, num,
									   FBF_LEAVE,
									   sizeof(LIST_ENTRY_T)*num,
									   plist );
	if( status != RESULT_FAIL )
	{
		// Передаём наш список ивентов непосредственно диалогу для обработки
		status = UIS_HandleEvent(papp->dialog, ev_st);
	}

	free(plist);

	return status;
}


UINT32 fbSendListItems( EVENT_STACK_T *ev_st,  void *app, UINT32 start, UINT32 num)
{
    APPLICATION_T			*papp = (APPLICATION_T*) app;
	LIST_ENTRY_T			*plist=NULL; // Буффер для элементов списка
	UINT32					i, index, status=RESULT_OK;
	UINT32					img_res=NULL, img_select=NULL;
    
    if( num==0 ) return RESULT_FAIL; // Контролируем параметр, на всякий случай
	
	plist = malloc( sizeof(LIST_ENTRY_T)*num ); // malloc
	if( plist==NULL ) return RESULT_FAIL;

    for( index=0, i=start; (i<start+num) && (i<=ENTRIES_NUM+1); i++, index++) {

        plist[index].editable = FALSE;			// Поле нередактируемо
		plist[index].content.static_entry.unk6 = 1;

        img_select=NULL;
        img_res=NULL;


        if (file_list[i-1].attrib & FS_ATTR_DIRECTORY) {
            img_res = DRMRES_FOLDER;
        } else {
			img_res = NULL;
		}	

        UIS_MakeContentFromString( "p1q0", &(plist[index].content.static_entry.text), file_list[i-1].name, img_res);

	}


    // Добавляем в текущий список ивентов новый ивент, предназначенный для нашего списка
	status = APP_UtilAddEvUISListData( ev_st, app, 0,
									   start, num,
									   FBF_LEAVE,
									   sizeof(LIST_ENTRY_T)*num,
									   plist );
	if( status != RESULT_FAIL )
	{
		// Передаём наш список ивентов непосредственно диалогу для обработки
		status = UIS_HandleEvent(papp->dialog, ev_st);
	}

	free(plist);

	return status;
}
*/

UINT32 mSelectItem( EVENT_STACK_T *ev_st,  void *app ) 
{
    UINT32 index = AFW_GetEv(ev_st)->data.index;

    switch (index) {
    case MENU_FILE_OPEN:
        APP_UtilChangeState(HE_STATE_FBROWSER, ev_st, app );
        break;
    case MENU_RAM_OPEN:
        APP_UtilChangeState(HE_STATE_RAM, ev_st, app );
        break;
    case MENU_SEEM_OPEN:
        APP_UtilChangeState(HE_STATE_SEEM, ev_st, app );
        break;
    case MENU_FIND:
        APP_UtilChangeState(HE_STATE_FIND, ev_st, app );
        break;
    case MENU_GOTO:
        APP_UtilChangeState(HE_STATE_GOTO, ev_st, app );
        break;
    }

	
	return RESULT_OK;
}

/*
UINT32 fbSelectItem( EVENT_STACK_T *ev_st,  void *app ) 
{

    UINT32 index = AFW_GetEv(ev_st)->data.index;
    
    UINT32          i;
    wchar_t           uri[FILEURI_MAX_LEN+1];
   

    	    if (index == 1 && u_strcmp(cur_folder, L"/"))
			{
                 debug_printf("Level Up", NULL);
				// назад
				if (cur_folder[wstrlen(cur_folder)-1] == '/') cur_folder[wstrlen(cur_folder)-1] = 0;
					for (i=wstrlen(cur_folder)-1; i > 0 ; i--)
					{
						if (cur_folder[i] == '/')
						{
							break;
						}
						else
						{
							cur_folder[i] = 0;
						}
					}

					FindFile( cur_folder, L"*");
	
					UpdateList(ev_st, app, 1);
                    UIS_Refresh();
			}
			else
			{
					if (file_list[index-1].attrib & FS_ATTR_DIRECTORY)
					{
                        debug_printf("Open folder", NULL);
                        //  открыть папку
   						wstrcat(cur_folder, file_list[index-1].name);
						wstrcat(cur_folder, L"/");
                        
						FindFile( cur_folder, L"*");

                        UpdateList(ev_st, app, 1);
                        UIS_Refresh();
					}
					else
					{
                        debug_printf("Open file", NULL);
                        // открыть файл
                        //wstrcpy(uri, L"file:/");
                        wstrcpy(uri, cur_folder);
                        wstrcat(uri, file_list[index-1].name);
                        OpenFile(uri);
                        LoadBuffer(0);

                        APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
					}
			}

	
	return RESULT_OK;
}

UINT32 UpdateList( EVENT_STACK_T *ev_st,  void *app, UINT32 sItem )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;

	if (sItem != 0) {
        debug_printf("Update List", NULL);

        DRM_SetResource( Resources[RES_LIST_CAPTION], (void*)cur_folder, (wstrlen(cur_folder)+1)*sizeof(wchar_t));

            APP_UtilAddEvUISListChange( ev_st,
                                    app,
                                    0, // = 0
                                    sItem,
                                    ENTRIES_NUM,
                                    TRUE,
                                    2,          // = 2
                                    FBF_LEAVE,
                                    NULL,
                                    NULL );	

            UIS_HandleEvent( papp->dialog,  ev_st );
    }
			
	
	return RESULT_OK;
}


UINT32 HandleUITokenGranted( EVENT_STACK_T *ev_st,  void *app )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UINT32                  status;

    // Вызываем дефолтный обработчик события
    status = APP_HandleUITokenGranted( ev_st, app );

    // Если всё хорошо, меняем текущий state
    if( (status == RESULT_OK) && (papp->token_status == 2) )
    {
        status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    }

    return status;
}
*/



UINT32 RAMStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UIS_DIALOG_T            dialog = NULL;
    wchar_t                   buf_offset[64];
    UINT8                   edit_type=1;

    wstrcpy(buf_offset, Lang.str[LNG_OPEN_RAM]);
    wstrcat(buf_offset, L" - ");

    if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

   switch(view_mode) {
   case TEXT_MODE:
   case HEX_MODE:
        edit_type = 1;
        wstrcat(buf_offset, L"HEX");
        break;

    case DEC_MODE:
        edit_type = 32; 
        wstrcat(buf_offset, L"DEC");
        break;

    }

    // заголовок 
    DRM_SetResource( Resources[RES_EDIT_CAPTION], (void*)buf_offset, (wstrlen(buf_offset)+1)*sizeof(wchar_t));


        dialog = UIS_CreateCharacterEditor( &papp->port,
                                            find_text,
                                            edit_type,
                                            255, // кол-во возможных символов
                                            FALSE,
                                            NULL,            
                                            Resources[RES_EDIT_CAPTION] );


        if(dialog == NULL) return RESULT_FAIL;

        papp->dialog = dialog;


        return RESULT_OK;
}

UINT32 RAMData( EVENT_STACK_T *ev_st,  void *app )
{
    UINT32       status = 0;
    EVENT_T      *event = AFW_GetEv(ev_st);
    char         buf[255];

    u_utoa(event->attachment, buf);

       switch(view_mode) {
       case HEX_MODE:
       case TEXT_MODE:
            OpenRAM(strtoul(buf,NULL,16));
            break;
    
       case DEC_MODE:
            OpenRAM(strtoul(buf,NULL,10));
            break;
       }


    status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    
    return status;
}

/*
UINT32 SEEMStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UIS_DIALOG_T            dialog = NULL;
    wchar_t                   buf_offset[64];
    UINT8                   edit_type=1;

    wstrcpy(buf_offset, RecLang(LNG_OPEN_SEEM));
    wstrcat(buf_offset, L" Record - ");

    if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

   switch(view_mode) {
   case TEXT_MODE:
   case HEX_MODE:
        edit_type = 1;
        wstrcat(buf_offset, L"HEX");
        break;

    case DEC_MODE:
        edit_type = 32; 
        wstrcat(buf_offset, L"DEC");
        break;
    }

    // заголовок 
    DRM_SetResource( Resources[RES_EDIT_CAPTION], (void*)buf_offset, (wstrlen(buf_offset)+1)*sizeof(wchar_t));


        dialog = UIS_CreateCharacterEditor( &papp->port,
                                            find_text,
                                            edit_type,
                                            255, // кол-во возможных символов
                                            FALSE,
                                            NULL,            
                                            Resources[RES_EDIT_CAPTION] );


        if(dialog == NULL) return RESULT_FAIL;
        papp->dialog = dialog;


        return RESULT_OK;
}

UINT32 SEEMData( EVENT_STACK_T *ev_st,  void *app )
{
    UINT32       status = 0;
    EVENT_T      *event = AFW_GetEv(ev_st);
    char         buf[255];
    char         *ptr_rec=NULL;
    UINT16        rec=0;


    u_utoa(event->attachment, buf);
    ptr_rec = strchr(buf, ' ');
    
       switch(view_mode) {
       case HEX_MODE:
       case TEXT_MODE:
            if (ptr_rec == NULL || ptr_rec == buf) rec = 1;
            else strtoul(ptr_rec+1,NULL,16);
            OpenSeem(strtoul(buf,NULL,16), rec);
            break;
    
       case DEC_MODE:
            if (ptr_rec == NULL || ptr_rec == buf) rec = 1;
            else strtoul(ptr_rec+1,NULL,10);
            OpenSeem(strtoul(buf,NULL,10), rec);
            break;
       }

       LoadBuffer(0);

    status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    
    return status;
}*/

UINT32 FindStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UIS_DIALOG_T            dialog = NULL;
    wchar_t                   buf_offset[64];
    UINT8                   edit_type=1;

    wstrcpy(buf_offset, Lang.str[LNG_FIND]);
    wstrcat(buf_offset, L" - ");
   

    if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

    switch(view_mode) {
    case HEX_MODE:
        edit_type = 1;
        wstrcat(buf_offset, L"HEX");
        break;

    case DEC_MODE:
        edit_type = 32; 
        wstrcat(buf_offset, L"DEC");
        break;

    case TEXT_MODE:
        edit_type = 1;
        wstrcat(buf_offset, L"TEXT");
        break;
    }

    // заголовок 
    DRM_SetResource( Resources[RES_EDIT_CAPTION], (void*)buf_offset, (wstrlen(buf_offset)+1)*sizeof(wchar_t));


        dialog = UIS_CreateCharacterEditor( &papp->port,
                                            find_text,
                                            edit_type,
                                            255, // кол-во возможных символов
                                            FALSE,
                                            NULL,            
                                            Resources[RES_EDIT_CAPTION] );


        if(dialog == NULL) return RESULT_FAIL;

        papp->dialog = dialog;


        return RESULT_OK;
}

UINT32 FindData( EVENT_STACK_T *ev_st,  void *app )
{
    UINT32       status = 0;
    EVENT_T      *event = AFW_GetEv(ev_st);
    char         buf[255];


    wstrcpy(find_text, event->attachment);

    Data2Pattern(event->attachment, buf);

    FindPattern(buf, offset);
    status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    
    return status;
}


UINT32 GoToStateEnter( EVENT_STACK_T *ev_st,  void *app,  ENTER_STATE_TYPE_T type )
{
    APPLICATION_T           *papp = (APPLICATION_T*) app;
    UIS_DIALOG_T            dialog = NULL;
    wchar_t                   buf_offset[64];
    UINT8                   edit_type=1;

    wstrcpy(buf_offset, Lang.str[LNG_GOTO]);
    wstrcat(buf_offset, L" - ");

    if(type!=ENTER_STATE_ENTER) 
		return RESULT_OK;

   switch(view_mode) {
   case HEX_MODE:
       
        edit_type = 1;
        wstrcat(buf_offset, L"HEX");
        break;

    case DEC_MODE:
        edit_type = 32; 
        wstrcat(buf_offset, L"DEC");
        break;

    case TEXT_MODE:
        edit_type = 1;
        wstrcat(buf_offset, L"TEXT");
        break;
    }

    // заголовок 
    DRM_SetResource( Resources[RES_EDIT_CAPTION], (void*)buf_offset, (wstrlen(buf_offset)+1)*sizeof(wchar_t));


        dialog = UIS_CreateCharacterEditor( &papp->port,
                                            find_text,
                                            edit_type,
                                            255, // кол-во возможных символов
                                            FALSE,
                                            NULL,            
                                            Resources[RES_EDIT_CAPTION] );


        if(dialog == NULL) return RESULT_FAIL;

        papp->dialog = dialog;


        return RESULT_OK;
}

int GoToData( void *mess,  void *app )
{
    int       status = 0;
    //EVENT_T      *event = AFW_GetEv(ev_st);
    char         buf[255];
    MSG_HEX_EDITOR *event = (MSG_HEX_EDITOR *)mess;

    //wstr2strn(event->attachment, buf, wstrlen(event->attachment) );

    int x;
    wtoi(event->attachment, wstrlen(event->attachment), &x);
    
       switch(view_mode)
       {
       case HEX_MODE:
       case TEXT_MODE:
            GoTo(strtol(buf,NULL,16));
            break;
    
       case DEC_MODE:
            GoTo(strtol(buf,NULL,10));
            break;
       }


    status = APP_UtilChangeState( HE_STATE_MAIN, ev_st, app );
    
    return status;
}

int Data2Pattern(wchar_t *data, char *pattern)
{
    char         hex[4];
    int       i=0, j=0;

    switch(view_mode)
    {
    case DEC_MODE:
    case HEX_MODE:
        wstr2strn( data, pattern, wstrlen(data) );

        for (i=0; i < strlen(pattern); i+=2)
        {
            strncpy(hex, pattern+i, 2);
            if (hex[0] == '?' || hex[1] == '?') pattern[j] = '?';
            else {
                //pattern[j] = (char)strtoul( hex, NULL, 16 ); // строка в  long!!!!!!!!!!!!!!!!!!
            }
            j++;
        }
        pattern[j] = 0;
        break;


    case TEXT_MODE:
        UCS2toCP1251(data, pattern);
        break;
    }

    return RESULT_OK;
}

/*
int FindFile( wchar_t *folder, wchar_t *filtr ) // поиск файлов
{
    INT32                   err=RESULT_OK;
	FS_SEARCH_PARAMS_T		sp;
	FS_SEARCH_RESULT_T		hResult;
	FS_SEARCH_HANDLE_T		hSearch;


	UINT16		count_files=0, count_res=1;
	UINT32		status = RESULT_OK;
	INT32		i=0, j=0;
    UINT8       mode=0;
    wchar_t       volumes[12];

    wchar_t       search_string[256];

    	
	if ( folder==NULL || !u_strcmp(folder, L"/")) {
            wstrcpy(search_string, L"/");
    } else {
        // определяем нужен ли file://
        if(wstrncmp(folder, L"file:/", 6))
        {
            wstrcpy(search_string, L"file:/");
            wstrcat(search_string, folder);
        } else {
            wstrcpy(search_string, folder);
        }
            
        // определяем нужен ли сплеш
        if ( folder[wstrlen(folder)-1] != '/' ) 
            wstrcat(search_string, L"/");
    
        wstrcat(search_string, L"\xFFFE");
        wstrcat(search_string, filtr);
    }

    ///// Особенности /////
    // 1) в корне диска `a` в hResult.name относительный путь!
    // 2) в списке может быть имя текущей папки. При этом в конце есть слэш. Последствия патча "удаление ненужных папок с тела"
    //////////////////////

    if (!u_strcmp(search_string, L"/")) { // диски

        DL_FsVolumeEnum(volumes);

        free(file_list);
        file_list = suAllocMem(sizeof(FILEINFO)*4, &err);
        if (err != RESULT_OK) return RESULT_FAIL;

        cur_folder[0] = '/';

        ENTRIES_NUM = 0;

        for (i=0; i < 4; i++) {
            file_list[i].name[0] = volumes[i*3+1];//'a' + i;
            file_list[i].name[1] = 0;
            file_list[i].attrib = FS_ATTR_DIRECTORY;

            ENTRIES_NUM++;

            if (volumes[i*3+2] == NULL)  break;
        }

        return RESULT_OK;
    }
	
	sp.flags = 0x1C; 
	sp.attrib = 0;
	sp.mask = 0;
	
	status = DL_FsSSearch( sp, search_string, &hSearch, &count_files, 0 );

	if ( status != RESULT_OK ) {
        DL_FsSearchClose( hSearch );
        return RESULT_FAIL;
    }

    debug_printf("FB: count_files = %d", count_files);
	
	ENTRIES_NUM = count_files+1;

	free(file_list);
    file_list = suAllocMem(sizeof(FILEINFO)*(count_files+1), &err);
    if (err != RESULT_OK) return RESULT_FAIL;
	
	wstrcpy(file_list[0].name, L"..");
	file_list[0].attrib = FS_ATTR_DIRECTORY;


	for ( i=0, j=1; j<=count_files; i++ ) {

            if (i >= count_files){
                if (mode > 0) {
                    break;
                } else {
                    i=-1;
                    mode=1;
                    continue;
                }
            }

		status = DL_FsSearchResults( hSearch, i, &count_res, &hResult );
		if ( status == RESULT_OK ) 
		{
            if ((mode==0) && !(hResult.attrib&FS_ATTR_DIRECTORY)) continue;
            if ((mode==1) && (hResult.attrib&FS_ATTR_DIRECTORY)) continue;

            // фиксим последствия  патча "удаление ненужных папок с тела"
            if(!u_strcmp(hResult.name+wstrlen(hResult.name)-1, L"/")) continue;
            	
			wstrcpy(file_list[j].name, SplitPath(hResult.name, L"/"));
			file_list[j].attrib = hResult.attrib;
			j++;
		} 
        
	}

	DL_FsSearchClose( hSearch ); 
	
	return RESULT_OK;
}
*/

int OpenFile(wchar_t *file_open)
{
    debug_printf("Open file");

    open_mode = OPEN_FILE;

    w_fclose( f);
    f = w_fopen( file_open, WA_READ, 0x1FF, 0);
    if (f == FILE_HANDLE_INVALID  ) return RESULT_FAIL;
    wstrcpy(_file_open, file_open);
    
    offset = 0;

    W_FSTAT w_fst;
	w_fstat(file_open, w_fst);
        file_size = w_fst.st_size;
        
    //file_size = DL_FsGetFileSize( f );
    debug_printf("file_size = %d", file_size);

    mfree(buffer);
    buffer = malloc(CELLS_COUNT);
    memset(buffer, 0, CELLS_COUNT); 

    SaveSetting(CFGfile);

    return RESULT_OK;
}

int OpenRAM(int  address)
{
    open_mode = OPEN_RAM;

    
    if (f != FILE_HANDLE_INVALID) free(buffer);
    w_fclose( f);
    f = FILE_HANDLE_INVALID;

    file_size = 0x7FFFFFFF;
    

    offset = address;    
    //buffer = (UINT8*)address;
    LoadBuffer(address);
    debug_printf("Open RAM = 0x%p", buffer);

    SaveSetting(CFGfile);

    return RESULT_OK;
}

/*
UINT32 OpenSeem(UINT16 seem, UINT32 rec)
{
    debug_printf("Open seem %x_%x", seem, record);

    open_mode = OPEN_SEEM;

    if (f != FILE_HANDLE_INVALID) free(buffer);
    w_fclose( f);
    f = FILE_HANDLE_INVALID;

    seem_id=seem; 
    if (SEEM_MAX_RECORD_get_max_record(seem) >= rec) record=rec;
    else record=rec;
    offset = 0;
    file_size = SEEM_GET_ADDRESS_LENGTH_element_length(seem);
    debug_printf("fseem_size = %d", file_size);

    free(buffer);
    buffer = (UINT8*)malloc(CELLS_COUNT);
    memset(buffer, 0, CELLS_COUNT); 

    SaveSetting(CFGfile);

    return RESULT_OK;
}
*/

int LoadBuffer(int off_set)
{
    int r;
    //SEEM_ELEMENT_DATA_CONTROL_T     seem_data;



    switch (open_mode) {

    case OPEN_FILE:
        if (f == FILE_HANDLE_INVALID) return RESULT_FAIL;
    
        w_lseek( f, (off_set/CELLS_COUNT)*CELLS_COUNT, WSEEK_SET );
        if (file_size < CELLS_COUNT) { 
            DL_FsReadFile( buffer,  1,  file_size, f, &r );
        } else {
            DL_FsReadFile( buffer,  1,  CELLS_COUNT, f, &r );
        }
        
        break;

    case OPEN_RAM:
        buffer = (int*)((off_set/CELLS_COUNT)*CELLS_COUNT);
        r = CELLS_COUNT;
        break;

    //case OPEN_SEEM:
    //    
    //    seem_data.seem_element_id = (UINT16)seem_id;
    //    seem_data.seem_record_number = (UINT16)record;
    //    seem_data.seem_offset = (UINT32)off_set;
    //    if (file_size < CELLS_COUNT) seem_data.seem_size = file_size;
    //    else seem_data.seem_size = CELLS_COUNT;
//
     //   memset(buffer, 0, CELLS_COUNT);
     //   r=SEEM_ELEMENT_DATA_read( &seem_data, buffer, TRUE);
     //   if (r == 0) r = CELLS_COUNT;
     //   break;

    }

    debug_printf("Read %d b from %d b", r, CELLS_COUNT);
    return r;
}

int SaveOffset(int off_set, int value)
{
    //SEEM_ELEMENT_DATA_CONTROL_T seem_element;
    //int w;

    debug_printf("Save offset 0x%x, new value = 0x%x", off_set, value);

    buffer[RelativeOffset(offset)] = value;

    switch (open_mode) {
    //case OPEN_SEEM:
    //    seem_element.seem_element_id = seem_id;
    //    seem_element.seem_record_number = record;
    //    seem_element.seem_offset = off_set;
    //    seem_element.seem_size = 1;
    //    SEEM_ELEMENT_DATA_write	( &seem_element, &value);	
    //    break;

    case OPEN_FILE:
        w_lseek( f,  off_set, WSEEK_SET );
        w_fwrite( f, &value, 1 );
        break;
        
    } 

    return RESULT_OK;
}

int GoTo(int off_set) 
{
    debug_printf("GoTo 0x%x", off_set);
    offset = off_set;
    LoadBuffer(off_set);

    return RESULT_OK;
}

int FindPattern(char *pattern, int off_set)
{
    int cur_offset = off_set;
    int begin_offset = RelativeOffset(off_set);  
    int offset_pattern = -1;
    int i=0, j=0;

    debug_printf("Find pattern = '%s'", pattern);

    while (cur_offset+i < file_size+CELLS_COUNT) {

        LoadBuffer(cur_offset);       

        for (i=begin_offset; i < CELLS_COUNT && cur_offset+i < file_size; i++) {
            debug_printf("sym buffer = 0x%x", buffer[i]);
            debug_printf("sym pattern = 0x%x", pattern[j]);
            if (buffer[i] == pattern[j] || pattern[j] == '?') {
                if (j < strlen(pattern)-1) {
                    debug_printf("Symbol %d found", pattern[j]);
                    j++;
                    if (offset_pattern == -1) offset_pattern = (cur_offset/CELLS_COUNT)*CELLS_COUNT+i;
                } else {
                    if (strlen(pattern)==1) offset_pattern = (cur_offset/CELLS_COUNT)*CELLS_COUNT+i; // поправка для односимвольной паттерны
                    debug_printf("Pattern found. offset = 0x%x", offset_pattern);
                    GoTo(offset_pattern); // паттерна найдена!
                    return offset_pattern;
                }
            } else {
                j=0;
                offset_pattern = -1;
            }
        }

        begin_offset = 0;
        cur_offset += CELLS_COUNT;
    }

    LoadBuffer(off_set);

    return offset_pattern;
}

int RelativeOffset(int off_set)
{
   int I;

    if (off_set >= CELLS_COUNT) {
        I = off_set % (CELLS_COUNT);
    } else {
        I = off_set;
    }

    return I;
}

void u_ltou(int intr, wchar_t *ws)
{
  snwprintf(ws, 0xFF, "%d", intr);
}


int paint(void)
{
    int  adr_major=0;
    int  addres=0;
    int  adr_minor=0;
    POINT  item;
    int  x, y;
    wchar_t   hex[16];
    wchar_t   info[64];
    int  i;

    adr_major = (offset/CELLS_COUNT)*CELLS_COUNT;
    addres = adr_major;
    
    // фон
	SetFillColor(0xFFFFFF00);
    FillRect(0, 0, bufd.w, bufd.h);

    // сетка
    SetForegroundColor(0x0000FF00);
    for (y=GRID_Y-LINE_H; y < bufd.h; y += LINE_H) {
        DrawLine(0, y, bufd.w, y);
    }

    for (x=GRID_X; x < bufd.w; x += COLUMN_W) {
        DrawLine(x, GRID_Y-LINE_H, x, y-LINE_H);
    }

    // информация
    SetForegroundColor(0x0000FF00);
    SetBackgroundColor(0xFFFFFFFF);
    info[0]=0;

    if (open_mode != OPEN_RAM) {
            // размер файла
        wstrcpy(info, L"s=");
        u_ltou(file_size, info+wstrlen(info));
        //snwprintf(info+wstrlen(info), 0xFF, "%d", file_size);

        // страница
        wstrcat(info, L"b; page=");
        u_ltou(adr_major/CELLS_COUNT+1, info+wstrlen(info));
        wstrcat(info, L"/");
        if (file_size%CELLS_COUNT > 0) {
            u_ltou(file_size/CELLS_COUNT+1, info+wstrlen(info));
        } else {
            u_ltou(file_size/CELLS_COUNT, info+wstrlen(info));
        }

        wstrcat(info, L"; ");
    }

    

    switch(view_mode)
    {
    case HEX_MODE:
        wstrcat(info, L"HEX");
        break;

    case DEC_MODE:
        wstrcat(info, L"DEC");
        break;

    case TEXT_MODE:
        wstrcat(info, L"TEXT");
        break;
    }

    DrawText(info, 1 , y-LINE_H+1, ANCHOR_LEFT | ANCHOR_TOP);

    // имя открытого файла
    if (open_mode == OPEN_FILE) {
        if (file_open[0] != 0 ) {
            DrawText( SplitPath(file_open, L"/"), 1 , 1, ANCHOR_LEFT | ANCHOR_TOP);
        } else {
            DrawText( L"HexEditor by om2804", 1 , 1, ANCHOR_LEFT | ANCHOR_TOP);
        }
    } else if (open_mode == OPEN_RAM) {
        DrawText( L"RAM", 1 , 1, ANCHOR_LEFT | ANCHOR_TOP);
    }
   
    // адреса
    SetForegroundColor(0x00000000);
    SetBackgroundColor(0xFFFFFFFF);
        // слева 
    for (y=0; y < LINES_COUNT; y++ ) {
        dec2hex( adr_major, hex, 8 );
        DrawText(hex, 1, GRID_Y + (LINE_H*y)+1, ANCHOR_LEFT | ANCHOR_TOP);
        adr_major = adr_major + 4;
    }
        // сверху
    for (x=0;  x < COLUMNS_COUNT; x ++) {
        dec2hex( adr_minor, hex, 2 );
        DrawText(hex, GRID_X + (COLUMN_W*x)+1, GRID_Y-(LINE_H-1), ANCHOR_LEFT | ANCHOR_TOP);
        adr_minor++;
    }

        // текущий оффсет
    SetForegroundColor(0xFF000000);
    dec2hex( offset, hex, 8 );
    DrawText(hex, 1, GRID_Y - (LINE_H-1), ANCHOR_LEFT | ANCHOR_TOP);

        // выделенная ячейка
    SetFillColor(0xBBBBBB00);
    item = XY(offset);
    FillRect(item.x+1, item.y+1, COLUMN_W-2, LINE_H-2);

    // заполняем данными
    if (buffer != NULL) {
        SetForegroundColor(0xFF000000);
        SetBackgroundColor(0xFFFFFFFF);
        x = 0;
        y = 0;
        for (i=0; i < CELLS_COUNT; i++) {
            if (open_mode==OPEN_FILE && addres+i >= file_size) break;
                switch (view_mode) {
                case HEX_MODE:
                    dec2hex( (UINT32)buffer[i], hex, 2 );
                    break;

                case DEC_MODE:
                    u_ltou(buffer[i], hex);
                    break;

                case TEXT_MODE:
                    hex[0] = buffer[i];
                    hex[1] = 0;
                    break;
                }
                
                DrawText(hex, GRID_X+(COLUMN_W*x)+1 , GRID_Y + (LINE_H*y)+1, ANCHOR_LEFT | ANCHOR_TOP);
        
                if (x < COLUMNS_COUNT-1) {
                    x++;
                } else {
                    x = 0;
                    y++;
                }
        }
    }

    

    return RESULT_OK;
}

POINT  XY(int sItem)
{
    POINT  item;
    int I=0;
    int n_line, n_column;


    I = RelativeOffset(offset);


    n_line = I/COLUMNS_COUNT;
    n_column = I - COLUMNS_COUNT*n_line;
    
    item.y = GRID_Y + n_line*LINE_H;
    item.x = GRID_X + n_column*COLUMN_W;


    return item;
}

//UINT32 HandleKeypress( EVENT_STACK_T *ev_st,  void *app )
void MyGuiOnKey(DISP_OBJ_HE *db, int key ,int, int repeat, int type)
{
    //EVENT_T     *event = AFW_GetEv(ev_st);

    //APP_ConsumeEv( ev_st, app );

    if(type == KBD_SHORT_RELEASE)
    {
       int       off_set=offset;
       char        buf[256];
      
    switch (key)
	{
        //case KEY_LEFT_SOFT:
        //case KEY_ESC: // выход
        //    return destroyApp( ev_st,  app );
	//		break;

        case KEY_DIGITAL_0+3:
            Data2Pattern(find_text, buf);
            FindPattern(buf, offset+wstrlen(find_text));
            break;

        case KEY_DIGITAL_0:
            if (view_mode < MAX_MODE) {
                view_mode++;
            } else {
                view_mode = 0;
            }
            break;

    //case KEY_POUND:
    case KEY_DIGITAL_0+9:
         if (open_mode==OPEN_RAM) {
             GoTo(offset+CELLS_COUNT);
         } else {
             GoTo((offset+CELLS_COUNT<file_size)?(offset+CELLS_COUNT):0);
         }
            break;
    
    case KEY_STAR:
        if (open_mode==OPEN_RAM) {
             GoTo(offset-CELLS_COUNT);
        } else {
             GoTo((offset-CELLS_COUNT>=0)?(offset-CELLS_COUNT):(file_size-1));
        }
            break;

    case KEY_DIGITAL_0+6:
    case KEY_RIGHT:
        if (open_mode==OPEN_RAM) {
            offset ++;
        } else {
            if(offset+1 < file_size) offset ++;
        }
            break;

	case KEY_DIGITAL_0+4:
        case KEY_LEFT:
            if (offset > 0) offset --;
            break;	

    case KEY_DIGITAL_0+8:
    case KEY_DOWN:
        if (open_mode==OPEN_RAM) {
            offset += COLUMNS_COUNT;
        } else {
            if(offset+COLUMNS_COUNT < file_size) offset += COLUMNS_COUNT;
        }
            break;	

         case KEY_DIGITAL_0+2:
         case KEY_UP:
            if (offset-COLUMNS_COUNT >= 0) {
                offset -= COLUMNS_COUNT;
            }
            break;

        case KEY_DIGITAL_0+5:
        case KEY_ENTER:
        case KEY_RIGHT_SOFT:
            //return APP_UtilChangeState(HE_STATE_EDIT, ev_st, app );
            BookObj_GotoPage(book, HE_STATE_EDIT);
            return(RESULT_OK);
            break;

        //case KEY_MENU:
        case KEY_DIGITAL_0+7:
            //return APP_UtilChangeState(HE_STATE_MENU, ev_st, app );
            BookObj_GotoPage(book, HE_STATE_MENU);
            return(RESULT_OK);
            break;
        }
    }
    
    if ((off_set/CELLS_COUNT) != (offset/CELLS_COUNT)) {
        LoadBuffer(offset);
    }

    paint();

    return RESULT_OK;
		
}

wchar_t* dec2hex( int dec, wchar_t *hex, int size )
{
    int d=dec;
    int len=0; // кол-во элементов hex_buf
	int hex_buf[32]; // остаток
	int i, j; // счётчик 


	if(d==0) {
        for (j=0; j < size; j++ ) {
            hex[j] = '0';
        }
		hex[j]= 0;

	} else {

        while (d != 0) {
            hex_buf[len] = d%16;
            d = d/16;
            len ++;
        }

        for (j=0; j < size - len; j++ ) {
            hex[j] = '0';
        }

        len--;
        for (i=len; i >= 0; i--) {
            if(hex_buf[i]<10) hex[j] = '0'+hex_buf[i];
            else hex[j] = 'A'+hex_buf[i]-10;
            j++;
        }   
        hex[j] = 0;
    }

	return hex;
}

// взять часть строки от spliter до конца
wchar_t* SplitPath( wchar_t* path, wchar_t* spliter ) 
{
	int j = wstrlen(path)-2;
	while( j > 0 ) 
	{
		if ( !wstrncmp(path+j, spliter, 1) ) 
		{
			return path+j+1;
		}
		j--;
	}
	return path;
}


int UCS2toCP1251(wchar_t *src, char *trg)
{
    int  i;

    for(i=0; i <= wstrlen(src); i++) {
        if (src[i] < 0xA0) { // до 128
            trg[i] = src[i];
        } else if (src[i] == 0x451){ // ё
            trg[i] = 0xB8;
        } else if (src[i] == 0x401){ // Ё
            trg[i] = 0xA8;
        } else if (src[i] > 0x40F) { // кирилица
            trg[i] = src[i] - 0x350;
        } else {
            trg[i] = 0x3F; // ? - нет символа
        }
    }

    return RESULT_OK;
}
