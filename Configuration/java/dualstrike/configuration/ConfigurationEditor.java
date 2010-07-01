package dualstrike.configuration;

import javax.swing.JFrame;

import dualstrike.configuration.model.Configuration;
import dualstrike.configuration.model.Info;

public class ConfigurationEditor {
	public static final String DEFAULT_CONFIGURATION_DEFINITION_FILE_URL = "configuration.xml";
	private final Configuration configuration;
	private final String language;
	private final String defaultLanguage;
	
	private ConfigurationEditor(final Configuration configuration, final String language) {
		this.configuration = configuration;
		
		if(language == null)
			this.language = configuration.getLang();
		else
			this.language = language;
		
		defaultLanguage = configuration.getLang();
	}
	
	public static ConfigurationEditor newInstance(String configurationDefinitionURL, final String language) throws ConfigurationDefinitionException {
		Configuration configuration;
		ConfigurationEditor ce;
		
		if(configurationDefinitionURL == null)
			configurationDefinitionURL = DEFAULT_CONFIGURATION_DEFINITION_FILE_URL;
		
		configuration = ConfigurationUtility.unmarshallConfiguration(configurationDefinitionURL);
		ce = new ConfigurationEditor(configuration, language);
		
		return ce;
	}
	
	public void init() {
		String title;
		String defaultTitle;
		JFrame frame;
		
		title = null;
		defaultTitle = null;
		
		for(Info currentTitle: configuration.getTitle()) {
			String currentLang;
			String value;
			
			currentLang = currentTitle.getLang();
			value = currentTitle.getValue();
			
			if(currentLang == null || currentLang.equals("")) {
				if(defaultLanguage.equals(language))
					title = value;
				else
					defaultTitle = value;
			}
			else if(currentLang.equals(language)) {
				title = value;
				break;
			}
		}
		
		if(title == null)
			title = defaultTitle;
		
		frame = new JFrame(title);

		//2. Optional: What happens when the frame closes?
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//4. Size the frame.
		frame.pack();

		//5. Show it.
		frame.setVisible(true);

	}
	
	public static void main(String[] args) {
		try {
			String configuration;
			String language;
			ConfigurationEditor ce;
			
			if(args.length == 0)
				configuration = null;
			else
				configuration = args[0];
				
			if(args.length > 1)
				language = args[1];
			else
				language = null;
			
			ce = ConfigurationEditor.newInstance(configuration, language);
			ce.init();
			System.exit(0);
		}
		catch(ConfigurationDefinitionException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}
}
