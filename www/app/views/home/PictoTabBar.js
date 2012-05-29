App.views.PictoTabBar = Ext.extend(Ext.TabBar, {
	dock: 'bottom',
	ui: 'dark',
	layout: {
		pack: 'center'
	},
	
	initComponent: function()
	{
		var thisComponent = this;
		this.previousTabIndex = 0;
		
		// iterate through all the items
		Ext.each(this.items, function(item)
		{
			// add a handler function for the tab button
			item.handler = function(){
				thisComponent.tabButtonHandler(this);
			};
			
		}, this);
		
		// detect when the route changed
		Ext.Dispatcher.on('dispatch', function(interaction)
		{
			var tabs = this.query('.tab');
			
			var action = interaction.action;
			var controller = interaction.controller.id;
			
			Ext.each(tabs, function(item)
			{
				if ( ! item.route)
					return;
				
				var dispatchOptions = Ext.Router.recognize(item.route);
				//
				var itemAction = dispatchOptions.action;
				var itemController = dispatchOptions.controller;
				
				if (itemController == controller && itemAction == action)
				{
					//this.setActiveTab(item);
					
					this.previousTabIndex = this.items.indexOf(item);
					
					return false;
				}
			}, this);
			
		}, this);
		
		// switch animation
		if ( ! this.switchAnimation)
		{
			this.switchAnimation = {type: 'slide'};
		}
		else if (Ext.isString(this.switchAnimation))
		{
			this.switchAnimation = {type: this.switchAnimation};
		}
		
		App.views.PictoTabBar.superclass.initComponent.apply(this, arguments);
	},
	
	// function called on tab button tap
	tabButtonHandler: function(tab)
	{
		var tabIndex = this.items.indexOf(tab);
		
		//this.setActiveTab(tab);
		
		if ( ! Ext.isEmpty(tab.route))
		{
			var anim = {};
			anim = Ext.apply(anim, this.switchAnimation);
			
			if (tabIndex != -1 && tabIndex < this.previousTabIndex)
			{
				anim.reverse = true;
			}
            
            if(tab.route!=="Scan")
            {
                var dispatchOptions = Ext.Router.recognize(tab.route);
                dispatchOptions.animation = anim;
                
                Ext.dispatch(dispatchOptions);
            }
            else
            {
                PGScanner.scan(["wtf"], 
                function()
                {
                    console.log("success");
                }, 
                function()
                {
                    console.log("fail");
                });
            }
		}
		
		this.previousTabIndex = tabIndex;
	},
	/* buggy...
	setActiveTab: function(tab)
	{ 
		var tabs = this.query('.tab');
	   
		Ext.each(tabs, function(item, index){
			item.removeCls('x-tab-active');
		}, this);
		
		tab.addCls('x-tab-active');
	}*/
});
Ext.reg('PictoTabBar', App.views.PictoTabBar);