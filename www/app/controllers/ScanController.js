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
                    //console.log(option.action);
                           
                    if(option.action.localeCompare("query") == 0)
                    {
                        console.log(option.query);
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