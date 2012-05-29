App.views.PictogramContextView = Ext.extend(Ext.Panel, {
	layout: {
		type: 'vbox',
		align: 'stretch'
	},
	scroll: 'auto',
    style: 'background: #d8efed',
    
    loadStore: function(query){
		var self = this;
        console.log('loadStore');
        Ext.Ajax.request({
                                url: 'http://98.207.1.60/ifs',
                                //callbackKey: 'callback',
                                params: { 'query':query, 'format':'json' },
//                                success: function (data) {
//                                    console.log("callback-ajax");
//                                    self.store.loadData(data);
//                                },
                                method: 'GET', //temporary fix
                                success: function(response, opts) {
//                                    console.log(response);
                                    console.log(response.responseText);
                                    var data = Ext.decode(response.responseText);
                                    console.log("data="+data);
                                    self.store.loadData(data);
                                },
                                failure:function(response, opts)
                                { 
                                    console.log("failure");
                                    console.log(response.responseText);
                                }
                           });
//    	Ext.util.JSONP.request({
//                               url: 'http://98.207.1.60/index_servlet.php',
//                               callbackKey: 'callback',
//                               params: { 'query':query, 'ver':'json' },
//                               callback: function (data) {
//                                    console.log("callback");
//                                    self.store.loadData(data);
//                               }
//                           });
    },
                                            
	initComponent: function(){
        console.log("initComponent");
		var self=this;
//		var pid = 'ef153517-9a11-46d1-8d16-6342d7dadb29';
		self.textPanel = new Ext.Panel({
 						height: 'vertical',
 						styleHtmlContent: true,
 					});
//		self.carousel = new Jarvus.mobile.PinchCarousel({
//			style: {
//				backgroundColor: '#000'
//			},
//			height: '360px',
//		});
//		self.items = [self.textPanel, self.carousel];
        self.items = [self.textPanel];
		
		self.store = new Ext.data.Store({
			panel: self,
			model: 'PictogramContextModel',

			listeners: {
 				single: true,
 				datachanged: function(){
                    console.log('datachanged');
 					self.store.each(function(imageRecord) {
// 						if(imageRecord.get('original'))
// 						{
//							self.carousel.addImage({
//								imageSrc:imageRecord.get('original'),
//								previewSrc: false
//							});
//						}
//						else if(imageRecord.get('text'))
//						{
//							self.textPanel.update(imageRecord.get('text'), true);
//							self.textPanel.doLayout();
//						}
                       if(imageRecord.get('dataset'))
                       {
							a = imageRecord.get('dataset');
							output = "";
							var i=0;
							for(i=0;i<a.length;i++)
							{
								output = output + (i+1)+" "+a[i][i+1] + "<br>";    
							}                       
                       }
                       else if(imageRecord.get('result'))
                       {
                       		output = output + "<br>" + imageRecord.get('result');
                       }
					});
					self.textPanel.update(output, true);
					self.textPanel.doLayout();
 				}
 			}
		});

		App.views.PictogramContextView.superclass.initComponent.apply(this, arguments);
	}
});
Ext.reg('PictogramContextView', App.views.PictogramContextView);