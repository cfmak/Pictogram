Ext.regController(
	'Scan', 
	{ 
        // index action
		index: function()
		{
            console.log('Scan/index');
			PGScanner.scan([""], 
                function (option)
                {
                    console.log("Scan/index : callback success");
                    console.log(option.action);
                    
                    //window.atob decode a string of base64
                    console.log(window.atob(option.query));
                           
                    if(option.action.localeCompare("query") == 0)
                    {
                           Ext.dispatch({
                                controller: 'Pictogram',
                                action    : 'context',
                                historyUrl: 'Scan/index',
                                query: option.query
                                });

//                           Ext.redirect('Pictogram/context');
                    }
                },
                function (){console.log("Scan/index : callback fail");});
		}
	}
);