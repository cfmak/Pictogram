Ext.regModel("PictogramContextModel", {
//	fields: [
//		{name: "text", type:"string"},
//		{name: "encoded", type:"string"},
//		{name: "original", type:"string"},
//		{name: "image", type:"string"}
//	],
     fields: [
              {name: "dataset", type:"array"},
              {name: "result", type:"int"}
              ],
	/*proxy: {
		type: 'ajax',
		reader: {
			root:'files',
			type:'json'
		}
	}*/
});