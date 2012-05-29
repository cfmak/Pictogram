App.views.HomeIndex = Ext.extend(Ext.Panel, {
/*
	initComponent: function(){
		var self = this;
		var takeButton = new Ext.Button({
			text: 'PictogramContext',
			ui: 'confirm',
			listeners: {tap: function() {
			}}
		});
		
		self.items = [takeButton];
		App.views.HomeIndex.superclass.initComponent.apply(this, arguments);
	}*/
	html:
		'<a href="#Pictogram/context" class="menu-item">PictogramContext</a><br>' +
		'<a href="#News/index" class="menu-item">News</a><br>' +
     	'<a href="#Home/about" class="menu-item">About</a><br>',
});
Ext.reg('HomeIndex', App.views.HomeIndex);