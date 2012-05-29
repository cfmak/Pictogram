//this is a class definition, not a declaration!
App.views.NewsDetails = Ext.extend(Ext.Panel, {
    scroll: 'vertical',
    styleHtmlContent: true,
    style: 'background: #d8efed',
 
    initComponent: function()
    {
        if (this.newsItem)
        {
            this.html = '';
            this.html += '<h3>'+ this.newsItem.get('title') +'</h3>';
            this.html += '<p>'+ this.newsItem.get('date') +'</p>';
            this.html += '<p>'+ this.newsItem.get('content') +'</p>';
        }
 
        App.views.NewsDetails.superclass.initComponent.apply(this, arguments);
    }
});
Ext.reg('NewsDetails', App.views.NewsDetails);