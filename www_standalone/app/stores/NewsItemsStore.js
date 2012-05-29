Ext.regStore('NewsItems', {
 
    model: 'NewsItem',
 
    proxy: {
        type: 'ajax',
        url : 'app/data/news.json',
    },
 	autoLoad: true
});