App.views.PictogramContextView = Ext.extend(Ext.Panel, {
	layout: {
		type: 'vbox',
		align: 'stretch'
	},
	scroll: 'vertical',
    style: 'background: #d8efed',
	initComponent: function(){
		var self=this;
//		var pid = 'ef153517-9a11-46d1-8d16-6342d7dadb29';
		self.textPanel = new Ext.Panel({
 						height: 'auto',
 						styleHtmlContent: true,
 					});
		self.carousel = new Jarvus.mobile.PinchCarousel({
			style: {
				backgroundColor: '#000'
			},
			height: '360px',
		});
		self.items = [self.textPanel, self.carousel];
		
		var store = new Ext.data.Store({
			panel: self,
			model: 'PictogramContextModel',
			proxy: {
				type: 'scripttag',
				//url: 'http://98.207.1.60/get.php?pid='+pid+'&ver=json',
                url: 'http://98.207.1.60/index_servlet.php?query='+pid+'&ver=json',
				reader: {
					type: 'json'
				}
			},

			listeners: {
 				single: true,
 				datachanged: function(){
 					store.each(function(imageRecord) {
 						if(imageRecord.get('original'))
 						{
							self.carousel.addImage({
								imageSrc:imageRecord.get('original'),
								previewSrc: false
							});
						}
						else if(imageRecord.get('text'))
						{
							self.textPanel.update(imageRecord.get('text'), true);
							self.textPanel.doLayout();
						}
//                       if(imageRecord.get('dataset'))
//                       {
//                           self.textPanel.update(imageRecord.get('dataset'), true);
//                           self.textPanel.doLayout();
//                       }
//                       else if(imageRecord.get('result'))
//                       {
//                           self.textPanel.update(imageRecord.get('text'), true);
//                           self.textPanel.doLayout();
//                       }
					});
 				}
 			}
		});
		store.read();
		
		App.views.PictogramContextView.superclass.initComponent.apply(this, arguments);
	}
});
Ext.reg('PictogramContextView', App.views.PictogramContextView);