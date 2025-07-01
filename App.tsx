/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React, {useState} from 'react';
import type {PropsWithChildren} from 'react';
import {
  ScrollView,
  StatusBar,
  StyleSheet,
  Text,
  TouchableOpacity,
  useColorScheme,
  View,
} from 'react-native';

import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';

import RunShellCommand from './app/specs/NativeRunShellCommand';

type SectionProps = PropsWithChildren<{
  title: string;
}>;

function Section({children, title}: SectionProps): React.JSX.Element {
  const isDarkMode = useColorScheme() === 'dark';
  return (
    <View style={styles.sectionContainer}>
      <Text
        style={[
          styles.sectionTitle,
          {
            color: isDarkMode ? Colors.white : Colors.black,
          },
        ]}>
        {title}
      </Text>
      <Text
        style={[
          styles.sectionDescription,
          {
            color: isDarkMode ? Colors.light : Colors.dark,
          },
        ]}>
        {children}
      </Text>
    </View>
  );
}

function App(): React.JSX.Element {
  const p = console.log;
  const shell = RunShellCommand.runSync;
  const shellAsync = RunShellCommand.runAsync;
  const [nodeVersion, setNodeVersion] = useState<string | null>(null);
  const [bunVersion, setBunVersion] = useState<string | null>(null);
  const arch = (global as any)?.nativeFabricUIManager ? 'Fabric' : 'Paper';

  const testRegularCommands = () => {
    p('Testing regular commands...');

    const p1: number = performance.now();
    const bun: string = RunShellCommand.runSync('bun --version').trim();
    const p1b: number = performance.now() - p1;

    const p2: number = performance.now();
    const node: string = RunShellCommand.runSync('node --version').trim();
    const p2b: number = performance.now() - p2;

    p(`Bun: ${bun} in ${p1b}ms`);
    p(`Node: ${node} in ${p2b}ms`);

    setNodeVersion(node);
    setBunVersion(bun);

    // Test async command
    const p3: number = performance.now();
    shellAsync("echo 'Hello from async command!'")
      .then((result: string) => {
        const p3b: number = performance.now() - p3;
        p(`Async result: ${result} in ${p3b}ms`);
      })
      .catch((error: any) => p(`Async error: ${error}`));
  };

  const isDarkMode = useColorScheme() === 'dark';

  const backgroundStyle = {
    backgroundColor: isDarkMode ? Colors.darker : Colors.lighter,
    flex: 1,
  };

  /*
   * To keep the template simple and small we're adding padding to prevent view
   * from rendering under the System UI.
   * For bigger apps the recommendation is to use `react-native-safe-area-context`:
   * https://github.com/AppAndFlow/react-native-safe-area-context
   *
   * You can read more about it here:
   * https://github.com/react-native-community/discussions-and-proposals/discussions/827
   */
  const safePadding = '5%';

  return (
    <View style={backgroundStyle}>
      <StatusBar
        barStyle={isDarkMode ? 'light-content' : 'dark-content'}
        backgroundColor={backgroundStyle.backgroundColor}
      />
      <ScrollView style={backgroundStyle}>
        <View style={{paddingRight: safePadding}}>
          <Header />
        </View>
        <View
          style={{
            backgroundColor: isDarkMode ? Colors.black : Colors.white,
            paddingHorizontal: safePadding,
            paddingBottom: safePadding,
          }}>
          <TouchableOpacity style={styles.button} onPress={testRegularCommands}>
            <Text style={styles.buttonText}>▶️ Run</Text>
          </TouchableOpacity>
          <Section title="Arch">{arch}</Section>
          <Section title="Bun Version">{bunVersion || 'Not detected'}</Section>
          <Section title="Node Version">
            {nodeVersion || 'Not detected'}
          </Section>
        </View>
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  button: {
    backgroundColor: 'orange',
    borderRadius: 8,
    paddingVertical: 12,
    paddingHorizontal: 18,
    alignSelf: 'flex-start',
    marginTop: 32,
    shadowColor: 'orange',
    shadowOpacity: 0.08,
    shadowRadius: 4,
    shadowOffset: {width: 0, height: 2},
  },
  buttonText: {
    color: '#FFF',
    fontWeight: 'bold',
    fontSize: 16,
    letterSpacing: 0.2,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: '600',
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: '400',
  },
  highlight: {
    fontWeight: '700',
  },
});

export default App;
