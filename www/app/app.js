Ext.regApplication({
    name: 'App',
	defaultUrl: 'Home/index',
    launch: function() {
    	this.launched = true;
        this.mainLaunch();
    }, //launch
    mainLaunch: function() {
		if (!device || !this.launched) {return;}
		this.viewport = new App.views.Viewport();
	}
});