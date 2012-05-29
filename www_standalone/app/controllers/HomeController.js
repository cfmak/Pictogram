Ext.regController(
	'Home', 
		{ 
			// index action
			index: function(options)
			{
				if ( ! this.indexView)
				{
					this.indexView = this.render({
						xtype: 'HomeIndex', //App.views.HomeIndex, app/views/home/HomeIndexView.js
						scroll: 'vertical',
						styleHtmlContent: true,
						style: 'background: #d8e2ef',
					});
				}
				var backBtn = this.application.viewport.query('#backBtn')[0];
				backBtn.hide();
				
				this.application.viewport.setActiveItem(this.indexView, options.animation);
			},
			
			// about action
			about: function()
			{
				if ( ! this.aboutView)
				{
					this.aboutView = this.render({
						xtype: 'HomeAbout',
					});
				}
				
				//get the instance of the back button 
				var backBtn = this.application.viewport.query('#backBtn')[0];
				backBtn.show();
				backBtn.setHandler(function()
				{
//					Ext.dispatch({
//						controller: 'Home',
//						action: 'index',
//						historyUrl: 'Home/index',
//						animation: {
//							type: 'slide',
//							reverse: true,
//						},
//					});
					Ext.dispatch({
                                controller: 'Pictogram',
                                action    : 'context',
                                historyUrl: 'Scan/index',
                                query: 'abc'
                                });
				});
			 
				this.application.viewport.setActiveItem(this.aboutView);
			},
		}
);