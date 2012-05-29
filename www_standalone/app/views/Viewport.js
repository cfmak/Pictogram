App.views.Viewport = Ext.extend(Ext.Panel, {
    fullscreen: true,
    layout: 'card',
    cardSwitchAnimation: 'slide',
    dockedItems: [
        {
            xtype: 'toolbar',
            title: 'Pictogram',
            items: [
            	{
            		text: 'Back',
            		itemId: 'backBtn',
            		ui: 'back',
            	}
            ],
        },
        {
            xtype: 'PictoTabBar',
            items: [
            	{
            		text: 'Scan',
					iconCls: 'scan',
					route: 'Scan/index'
            	},
            	{
            		text: 'Browse',
					iconCls: 'browse',
					route: 'Home/index'
            	}
            ],
            itemId: 'tabBar' //so that i can refer to it and hide it
        },
    ],
});