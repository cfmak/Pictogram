Ext.regController('Pictogram', {
 
    // index action
    context: function(options)
    {
//        if ( ! this.contextPanel)
//        {
            this.contextPanel = this.render({
				xtype: 'PictogramContextView',
				//query: options.query,
                //scroll: 'vertical',
				//styleHtmlContent: true,
				//style: 'background: #d8e2ef',
                /*listeners: {
					itemtap: function(view, index){
						this.selectedNewsItem = view.store.getAt(index);
						Ext.redirect('somewhere');
					},
					scope: this
				}*/
            });
            this.contextPanel.loadStore(options.query);
//            this.contextPanel.pid = '4e6c8e63-2706-43a0-b617-f023b3028fdf';
//            this.contextPanel.query = options.query;
//        }
 
        var backBtn = this.application.viewport.query('#backBtn')[0];
        backBtn.show();
 
        backBtn.setHandler(function()
        {
            Ext.dispatch({
                controller: 'Home',
                action: 'index',
                historyUrl: 'Home/index',
                //
                animation: {
                    type: 'slide',
                    reverse: true,
                },
            });
        });
 
        this.application.viewport.setActiveItem(this.contextPanel, options.animation);
    },
});