Ext.regController('News', {
 
    // index action
    index: function(options)
    {
        if ( ! this.indexView)
        {
            this.indexView = this.render({
                xtype: 'NewsIndex',
                listeners: {
					itemtap: function(view, index){
						this.selectedNewsItem = view.store.getAt(index);
						Ext.redirect('News/details');
					},
					scope: this
				}
            });
        }
 
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
 
        this.application.viewport.setActiveItem(this.indexView, options.animation);
    },
    
    details: function(options)
	{
		if ( ! this.selectedNewsItem)
		{
			Ext.redirect('News/index');
			return;
		}
	 
		if ( ! this.detailsView)
		{
			this.detailsView = this.render({
				xtype: 'NewsDetails',
				newsItem: this.selectedNewsItem,
				listeners: {
					deactivate: function(view){
						view.destroy();
						delete this.detailsView;
					},
					scope: this
				}
			});
		}
	 
		var backBtn = this.application.viewport.query('#backBtn')[0];
		backBtn.show();
		backBtn.setHandler(function()
		{
			Ext.dispatch({
				controller: 'News',
				action: 'index',
				historyUrl: 'News/index',
				//
				animation: {
					type: 'slide',
					reverse: true,
				},
			});
		});
		this.application.viewport.setActiveItem(this.detailsView, options.animation);
	}
});