App.views.NewsIndex = Ext.extend(Ext.List, {
    store: 'NewsItems',
    itemTpl: '{title}, <span class="date">{date}</date>',
});
Ext.reg('NewsIndex', App.views.NewsIndex);